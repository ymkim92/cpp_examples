/***************************************************************************
 Copyright (c) 2009 Stephen Roderick <xxxstephen AT theptrgroupxxx DOT comxxx>
                                             (remove the x's above)

 ***************************************************************************
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Lesser General Public            *
 *   License as published by the Free Software Foundation; either          *
 *   version 2.1 of the License, or (at your option) any later version.    *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   Lesser General Public License for more details.                       *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library; if not, write to the Free Software   *
 *   Foundation, Inc., 59 Temple Place,                                    *
 *   Suite 330, Boston, MA  02111-1307  USA                                *
 *                                                                         *
 ***************************************************************************/

/** \file OneAxisFilter.hpp
	Component that filters out one axis (ie sets it to zero)
*/

#ifndef	__ONEAXISFILTER_HPP
#define	__ONEAXISFILTER_HPP 1

#include <rtt/TaskContext.hpp>
#include <rtt/Ports.hpp>
#include <kdl/frames.hpp>

/** Component that filters out one axis (ie sets it to zero)
 */
class OneAxisFilter : public RTT::TaskContext
{
protected:
	// DATA INTERFACE

	// *** INPUTS ***

	/// desired cartesian position
	RTT::ReadDataPort<KDL::Frame>			inputPosition_port;

	// *** OUTPUTS ***

	/// desired cartesian position
	RTT::WriteDataPort<KDL::Frame>			outputPosition_port;
	
	// *** CONFIGURATION ***

	/// specify which axis to filter (should be one of "x", "y", or "z")
	RTT::Property<std::string>				axis_prop;
	
public:
	OneAxisFilter(std::string name);
	virtual ~OneAxisFilter();

protected:
	/// validate axis_prop value
	/// \return true if axis_prop value is valid, otherwise false
	virtual bool configureHook();
	/// filter one translational axis (as specified by axis_prop)
	virtual void updateHook();
};

#endif
