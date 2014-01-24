/*
 * IBSONRenderer.hpp
 *
 *  Created on: Jan 23, 2014
 *      Author: mdeazley
 */

#ifndef IBSONRENDERER_HPP_
#define IBSONRENDERER_HPP_

#include <ostream>
#include <memory>

namespace mongotype {

class IBSONRenderer {
public:
	virtual ~IBSONRenderer() {};
	virtual std::ostream& render(std::ostream& os) = 0;
};

} /* namespace mongotype */

#endif /* IBSONRENDERER_HPP_ */
