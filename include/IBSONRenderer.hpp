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
	virtual void setOutputStream(std::ostream& os) = 0;
	virtual void begin(const char* prefix) = 0;
	virtual void end(const char* suffix) = 0;
	virtual void render(const BSONObj& object, int docIndex, int docCount) = 0;
};

} /* namespace mongotype */

#endif /* IBSONRENDERER_HPP_ */
