/******************************************************************************
Copyright (c) 2017, Farbod Farshidian. All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of the copyright holder nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
******************************************************************************/

#ifndef ROLLOUT_SETTINGS_OCS2_H_
#define ROLLOUT_SETTINGS_OCS2_H_

#include <string>
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/info_parser.hpp>

#include <ocs2_core/Dimensions.h>
#include <ocs2_core/integration/Integrator.h>

namespace ocs2{

/**
 * This structure contains the settings for forward rollout algorithms.
 */
class Rollout_Settings {

public:
	typedef Dimensions<0,0>::RICCATI_INTEGRATOR_TYPE RICCATI_INTEGRATOR_TYPE;

	/**
	 * Default constructor.
	 */
	Rollout_Settings()
	: Rollout_Settings(1e-9, 1e-6, 5000, 1e-3, IntegratorType::ODE45, false)
	{}

	/**
	 * Constructor with all settings as arguments.
	 *
	 * @param [in] absTolODE: Absolute tolerance of the rollout.
	 * @param [in] relTolODE: Relative tolerance of the rollout.
	 * @param [in] maxNumStepsPerSecond: Maximum number of steps in the rollout.
	 * @param [in] minTimeStep: Minimum time step of the rollout.
	 * @param [in] integratorType: Rollout integration scheme type.
	 * @param [in] checkNumericalStability: Whether to check that the rollout is numerically stable.
	 */
	Rollout_Settings(
			double absTolODE,
			double relTolODE,
			size_t maxNumStepsPerSecond,
			double minTimeStep,
			IntegratorType integratorType,
			bool checkNumericalStability)
	: absTolODE_(absTolODE)
	, relTolODE_(relTolODE)
	, maxNumStepsPerSecond_(maxNumStepsPerSecond)
	, minTimeStep_(minTimeStep)
	, integratorType_(integratorType)
	, checkNumericalStability_(checkNumericalStability)
	{}

	/**
	 * This function loads the "Rollout_Settings" variables from a config file. This file contains the settings for the Rollout algorithms.
	 * Here, we use the INFO format which was created specifically for the property tree library (refer to www.goo.gl/fV3yWA).
	 *
	 * It has the following format:	<br>
	 * rollout	<br>
	 * {	<br>
	 *   absTolODE                value		<br>
	 *   relTolODE                value		<br>
	 *   maxNumStepsPerSecond     value		<br>
	 *   minTimeStep              value		<br>
	 *   (and so on for the other fields)	<br>
	 * }	<br>
	 *
	 * If a value for a specific field is not defined it will set to the default value defined in "Rollout_Settings".
	 *
	 * @param [in] filename: File name which contains the configuration data.
	 * @param [in] fieldName: Field name which contains the configuration data.
	 * @param [in] verbose: Flag to determine whether to print out the loaded settings or not (The default is true).
	 */
	void loadSettings(const std::string& filename, const std::string& fieldName, bool verbose = true);

public:
	/****************
	 *** Variables **
	 ****************/
	/** This value determines the absolute tolerance error for ode solvers. */
	double absTolODE_;
	/** This value determines the relative tolerance error for ode solvers. */
	double relTolODE_;
	/** This value determines the maximum number of integration points per a second for ode solvers. */
	size_t maxNumStepsPerSecond_;
	/** The minimum integration time step */
	double minTimeStep_;
	/** Rollout integration scheme type */
	IntegratorType integratorType_;
	/** Whether to check that the rollout is numerically stable */
	bool checkNumericalStability_;

}; // end of Rollout_Settings class


inline void Rollout_Settings::loadSettings(const std::string& filename, const std::string& fieldName, bool verbose /*= true*/) {

	boost::property_tree::ptree pt;
	boost::property_tree::read_info(filename, pt);

	if(verbose){
		std::cerr << std::endl << " #### Rollout Settings: " << std::endl;
		std::cerr <<" #### =============================================================================" << std::endl;
	}

	try	{
		absTolODE_ = pt.get<double>(fieldName + ".AbsTolODE");
		if (verbose)  std::cerr << " #### Option loader : option 'AbsTolODE' ........................... " << absTolODE_ << std::endl;
	}
	catch (const std::exception& e){
		if (verbose)  std::cerr << " #### Option loader : option 'AbsTolODE' ........................... " << absTolODE_ << "   \t(default)" << std::endl;
	}

	try	{
		relTolODE_ = pt.get<double>(fieldName + ".RelTolODE");
		if (verbose)  std::cerr << " #### Option loader : option 'RelTolODE' ........................... " << relTolODE_ << std::endl;
	}
	catch (const std::exception& e){
		if (verbose)  std::cerr << " #### Option loader : option 'RelTolODE' ........................... " << relTolODE_ << "   \t(default)" << std::endl;
	}

	try	{
		maxNumStepsPerSecond_ = pt.get<double>(fieldName + ".maxNumStepsPerSecond");
		if (verbose)  std::cerr << " #### Option loader : option 'maxNumStepsPerSecond' ................ " << maxNumStepsPerSecond_ << std::endl;
	}
	catch (const std::exception& e){
		if (verbose)  std::cerr << " #### Option loader : option 'maxNumStepsPerSecond' ................ " << maxNumStepsPerSecond_ << "   \t(default)" << std::endl;
	}

	try	{
		minTimeStep_ = pt.get<double>(fieldName + ".minTimeStep");
		if (verbose)  std::cerr << " #### Option loader : option 'minTimeStep' ......................... " << minTimeStep_ << std::endl;
	}
	catch (const std::exception& e){
		if (verbose)  std::cerr << " #### Option loader : option 'minTimeStep' ......................... " << minTimeStep_ << "   \t(default)" << std::endl;
	}

	try	{
		integratorType_ = (IntegratorType)pt.get<int>(fieldName + ".integratorType");
		if (verbose)  std::cerr << " #### Option loader : option 'integratorType' ...................... " << ocs2::to_string(integratorType_) << std::endl;
	}
	catch (const std::exception& e){
		if (verbose)  std::cerr << " #### Option loader : option 'integratorType' ...................... " << ocs2::to_string(integratorType_) << "   \t(default)" << std::endl;
	}

	try	{
		checkNumericalStability_ = pt.get<bool>(fieldName + ".checkNumericalStability");
		if (verbose)  std::cerr << " #### Option loader : option 'checkNumericalStability' ............. " << checkNumericalStability_ << std::endl;
	}
	catch (const std::exception& e){
		if (verbose)  std::cerr << " #### Option loader : option 'checkNumericalStability' ............. " << checkNumericalStability_ << "   \t(default)" << std::endl;
	}

	if(verbose)
		std::cerr <<" #### =============================================================================" << std::endl;
}

} // namespace ocs2

#endif /* ROLLOUT_SETTINGS_OCS2_H_ */
