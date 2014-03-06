/*
 * BSONTypeFormatter.hpp
 *
 *  Created on: Jan 24, 2014
 *      Author: mdeazley
 */

#ifndef BSONTYPEFORMATTER_HPP_
#define BSONTYPEFORMATTER_HPP_

#include <mongotype.hpp>
#include <Parameters.hpp>
#include <BSONTypeMap.hpp>

namespace mongotype {

//----------------------------------------------------------------------------

/*!
 * \brief BSON Element Type String Lookup
 *
 * Provides conversion from BSON integer element type codes to human readable strings.
 */

class BSONTypeFormatter : public BSONTypeMap {

	Parameters& params;

public:

	/*!
	 * \brief Construct a type lookup instance.
	 * \param[in] pparams The command line parameters object.
	 * \param[in] e The BSONElement whose type string will be looked up.
	 */
	BSONTypeFormatter(Parameters& pparams, const BSONElement& e) :
			BSONTypeMap(e), params(pparams) {
	}

	/*!
	 * \brief BSON Object Type Strings Stream Output Operator.
	 * \param[in,out] out The std::ostream to write the BSON type strings.
	 * \param[in] bos The BSONTypeFormatter object to write to the output stream.
	 */
	string to_string() {
		string s;
		if (params.getTypeMask()) {
			const BSONTypeElement& bsts = lookup();
			s = "(";
			if (params.getTypeMask() & TYPE_NAME) {
				s += bsts.getName();
			}
			if (params.getTypeMask() & TYPE_DESC) {
				if (s.length()) {
					s += "/";
				}
				s += bsts.getDesc();
			}
			if (params.getTypeMask() & TYPE_CODE) {
				if (s.length()) {
					s += "/";
				}
				s += std::to_string((int)(element.type()));
			}
			s += ")";
		}
		return s;
	}

	/*!
	 * \brief BSON Object Type Strings Stream Output Operator.
	 * \param[in,out] out The std::ostream to write the BSON type strings.
	 * \param[in] bstf The BSONTypeFormatter object to write to the output stream.
	 */
	OSTREAM_FRIEND(BSONTypeFormatter& bstf) {
		return out << bstf.to_string();
	}
};

} /* namespace mongotype */

#endif /* BSONTYPEFORMATTER_HPP_ */
