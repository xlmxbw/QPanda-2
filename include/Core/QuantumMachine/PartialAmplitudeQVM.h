/*! \file PartialAmplitudeQVM.h */
#ifndef  _PARTIALAMPLITUDE_H_
#define  _PARTIALAMPLITUDE_H_
#include "Core/Utilities/Tools/Utils.h"
#include "Core/Utilities/Tools/Uinteger.h"
#include "Core/Utilities/Tools/Traversal.h"
#include "Core/QuantumMachine/OriginQuantumMachine.h"
#include "Core/VirtualQuantumProcessor/MPSQVM/MPSImplQPU.h"
#include "Core/VirtualQuantumProcessor/PartialAmplitude/PartialAmplitudeGraph.h"
#include "Core/Utilities/QProgTransform/TransformDecomposition.h"
#include "Core/Utilities/QProgInfo/Visualization/QVisualization.h"
#include <Core/Utilities/Compiler/QProgToOriginIR.h>

QPANDA_BEGIN

/**
* @class PartialAmplitudeQVM   
* @ingroup QuantumMachine
* @see QuantumMachine
* @brief Quantum machine for partial amplitude simulation
*/

enum class BackendType 
{
    CPU,
    GPU,
    CPU_SINGLE_THREAD,
    NOISE,
    MPS
};

class PartialAmplitudeQVM : public QVM, public TraversalInterface<>
{
public:
	PartialAmplitudeGraph m_graph_backend;

    void init(BackendType type = BackendType::CPU);

    std::map<std::string, bool> directlyRun(QProg &prog, const NoiseModel& noise_model= NoiseModel()) override
    {
        run(prog);
        return std::map<std::string, bool>();
    }

    int get_spilt_num(QProg &node)
    {
        auto qubit_num = getAllocateQubitNum();
        m_graph_backend.reset(qubit_num);
        execute(node.getImplementationPtr(), nullptr);
        return m_graph_backend.m_spilt_num;
    }

	template <typename _Ty>
	void run(_Ty &node, const NoiseModel& noise_model= NoiseModel())
	{
		auto qubit_num = getAllocateQubitNum();
		m_graph_backend.reset(qubit_num);
		execute(node.getImplementationPtr(), nullptr);
		construct_graph();
	}

	/**
	* @brief  PMeasure by binary index
	* @param[in]  std::string  binary index
	* @return     qstate_type double
	* @note  example: PMeasure_bin_index("0000000000")
	*/
	qcomplex_t pmeasure_bin_index(std::string);

	/**
	* @brief  PMeasure by decimal  index
	* @param[in]  std::string  decimal index
	* @return     qstate_type double
	* @note  example: PMeasure_dec_index("1")
	*/
	qcomplex_t pmeasure_dec_index(std::string);


    /**
    * @brief    PMeasure_subset
    * @param[in]  QProg  qubits vec
    * @param[in]  std::vector<std::string>
    * @return     prob_map std::map<std::string, qstate_type>
    * @note  output example: <0000000110:0.000167552>
    */
	stat_map pmeasure_subset(const std::vector<std::string>&);

    prob_dict getProbDict(const QVec&);
    prob_dict probRunDict(QProg &prog, const QVec&);

    prob_vec getProbList(const QVec&);
    prob_vec probRunList(QProg &, const QVec&);

    void execute(std::shared_ptr<AbstractQGateNode>, std::shared_ptr<QNode>);
    void execute(std::shared_ptr<AbstractClassicalProg>, std::shared_ptr<QNode>);
    void execute(std::shared_ptr<AbstractQuantumMeasure>, std::shared_ptr<QNode>);
	void execute(std::shared_ptr<AbstractQuantumReset>, std::shared_ptr<QNode>);
    void execute(std::shared_ptr<AbstractQuantumCircuit>, std::shared_ptr<QNode>);
    void execute(std::shared_ptr<AbstractQuantumProgram>, std::shared_ptr<QNode>);
    void execute(std::shared_ptr<AbstractControlFlowNode>, std::shared_ptr<QNode>);
    void execute(std::shared_ptr<AbstractQNoiseNode>, std::shared_ptr<QNode>);
    void execute(std::shared_ptr<AbstractQDebugNode>, std::shared_ptr<QNode>);

    virtual size_t get_processed_qgate_num() override { throw std::runtime_error("not implementd yet"); }
	virtual void async_run(QProg& qProg, const NoiseModel& = NoiseModel()) override { throw std::runtime_error("not implementd yet"); }
	virtual bool is_async_finished() override { throw std::runtime_error("not implementd yet"); }
    virtual std::map<std::string, bool> get_async_result() override { throw std::runtime_error("not implementd yet"); }

private:
	void construct_graph();
	void computing_graph(int qubit_num, const cir_type& circuit, QStat& state);
    void caculate_qstate(QStat &state);
    void construct_qnode(int gate_type,
                         bool is_dagger,
                         const std::vector<size_t>& qubits,
                         const std::vector<double>& params);

    std::shared_ptr<QPUImpl> m_simulator = nullptr;
};

QPANDA_END
#endif

