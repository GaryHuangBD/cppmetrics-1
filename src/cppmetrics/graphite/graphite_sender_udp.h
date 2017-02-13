/*
 * GraphiteSenderUDP.h
 *
 *  Created on: Jul 27, 2016
 *      Author: noam
 */

#ifndef SRC_CPPMETRICS_GRAPHITE_GRAPHITE_SENDER_UDP_H_
#define SRC_CPPMETRICS_GRAPHITE_GRAPHITE_SENDER_UDP_H_
#include <boost/cstdint.hpp>
#include <boost/asio.hpp>
#include "cppmetrics/graphite/graphite_sender.h"

namespace cppmetrics {
namespace graphite {

class GraphiteSenderUDP: public GraphiteSender {
	public:
	    /**
	     * Creates a new sender with the given params.
	     * @param host The graphite server host.
	     * @param port The graphite server port.
	     */
		GraphiteSenderUDP(const std::string& host, uint16_t port);
	    virtual ~GraphiteSenderUDP();

	    /**
	     * Connects to the graphite sender over UDP.
	     * does nothing.
	     * @throws nothing
	     */
	    virtual void connect();

	    /**
	     * Posts the metric name, value and timestamp to the graphite server.
	     * @param name The name of the metric
	     * @param value The value of the metric
	     * @param timestamp The timestamp of the metric.

	     * @throws std::runtime_error if there is a problem.
	     */
	    virtual void send(const std::string& name,
	            const std::string& value,
	            boost::uint64_t timestamp, metric_t type );

	    /**
	     * Closes the connection.
	     * nothing to do for UDP
	     */
	    virtual void close();

	private:
	    bool connected_;
	    std::string host_;
	    uint16_t port_;

	    boost::asio::io_service io_service_;
	    boost::asio::ip::udp::udp::endpoint receiver_endpoint_;
	    boost::asio::ip::udp::udp::socket	socket_;
};

}} // namespace

#endif /* SRC_CPPMETRICS_GRAPHITE_GRAPHITE_SENDER_UDP_H_ */
