#pragma once

#include <memory>
#include "AbstractQDebugNode.h"

QPANDA_BEGIN

class QDebug : public AbstractQDebugNode
{
private:
    std::shared_ptr<AbstractQDebugNode> m_debug_node;

public:
    QDebug(std::shared_ptr<AbstractQDebugNode> origin_debug)
        : m_debug_node(origin_debug)
    {
    }
    virtual ~QDebug() {}
    virtual void save_qstate(QStat& stat) override
    {
        m_debug_node->save_qstate(stat);
    }

    std::shared_ptr<AbstractQDebugNode> getImplementationPtr()
    {
        return m_debug_node;
    }
};

QPANDA_END