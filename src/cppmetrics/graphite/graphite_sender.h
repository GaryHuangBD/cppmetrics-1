/*
 * Copyright 2000-2014 NeuStar, Inc. All rights reserved.
 * NeuStar, the Neustar logo and related names and logos are registered
 * trademarks, service marks or tradenames of NeuStar, Inc. All other
 * product names, company names, marks, logos and symbols may be trademarks
 * of their respective owners.
 */

/*
 * graphite_sender.h
 *
 *  Created on: Jun 11, 2014
 *      Author: vpoliboy
 */

#ifndef GRAPHITE_SENDER_H_
#define GRAPHITE_SENDER_H_

#include <memory>

namespace cppmetrics {
namespace graphite {

/**
 * Interface to all graphite senders using different protocols like TCP UDP and
 * AMQP etc.
 */
class GraphiteSender {
public:
    // HACK: the sender should not have to know the metric type.
    // this is added for dd-statsd format needs
    enum class metric_t { Gauge_t, Counter_t, Histogram_t };

    virtual ~GraphiteSender() {}

    /**
     * Connects to the graphite sender
     * @return True on success, false otherwise.
     * @throws std::runtime_error if there is a problem.
     */
    virtual void connect() = 0;

    /**
     * Posts the metric name, value and timestamp to the graphite server.
     * @param name The name of the metric
     * @param value The value of the metric
     * @param timestamp The timestamp of the metric.
     * @return True on success false otherwise.
     * @throws std::runtime_error if there is a problem.
     */
    virtual void send(const std::string &name, const std::string &value,
        uint64_t timestamp, metric_t type = metric_t::Counter_t) = 0;

    /**
     * Closes the connection.
     */
    virtual void close() = 0;
};

typedef std::shared_ptr<GraphiteSender> GraphiteSenderPtr;

} /* namespace graphite */
} /* namespace cppmetrics */
#endif /* GRAPHITE_SENDER_H_ */
