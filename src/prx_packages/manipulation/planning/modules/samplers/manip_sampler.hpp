/**
 * @file manipulation_sampler.hpp
 *
 * @copyright Software License Agreement (BSD License)
 * Copyright (c) 2013, Rutgers the State University of New Jersey, New Brunswick
 * All Rights Reserved.
 * For a full description see the file named LICENSE.
 *
 * Authors: Andrew Dobson, Andrew Kimmel, Athanasios Krontiris, Zakary Littlefield, Kostas Bekris
 *
 * Email: pracsys@googlegroups.com
 */
#pragma once

#ifndef PRX_MANIP_SAMPLER_HPP
#define PRX_MANIP_SAMPLER_HPP

#include "prx/planning/modules/samplers/sampler.hpp"
#include "../../../../baxter/simulation/plants/manipulator.hpp"
#include "simulation/plants/movable_body_plant.hpp"


namespace prx
{
    namespace packages
    {
        namespace manipulation
        {
            using namespace baxter;
            /**
             * Performs sampling for the manipulator over a plane. It will randomly compute
             * x,y and a theta, but it will return the state space point for the manipulator.
             *
             * @brief <b> Performs sampling for the manipulator over a plane.</b>
             *
             * @author Athanasios Krontiris
             */
            class manip_sampler_t : public plan::sampler_t
            {

              public:

                manip_sampler_t();

                virtual ~manip_sampler_t();

                /**
                 * @copydoc sampler_t::init(const parameter_reader_t* , const parameter_reader_t*)
                 */
                virtual void init(const util::parameter_reader_t* reader, const util::parameter_reader_t* template_reader = NULL);

                /**
                 * @copydoc sampler_t::sample(const space_t*, space_point_t*)
                 */
                virtual void sample(const util::space_t* space, util::space_point_t* point);

                /**
                 * The near_point represent the object state that we need to grasp. The space and the point will be in the
                 * full state space that both manipulator and object belong to the planning space.
                 *
                 * @copydoc sampler_t::sample_near(const space_t*, space_point_t*, std::vector<bounds_t*>&, space_point_t*)
                 */
                virtual void sample_near(const util::space_t* space, util::space_point_t* near_point, std::vector<util::bounds_t*>& bounds, util::space_point_t* point);

                /**
                 * The target_point represents the object state that we need to grasp. The space and the point will be in the
                 * full state space that both manipulator and object belong to the planning space.
                 *
                 * @copydoc sampler_t::sample_near(const space_t*, space_point_t*, std::vector<bounds_t*>&, space_point_t*)
                 */
                virtual bool sample_near_object(sim::state_t* result_point, const sim::state_t* target_point);

                /**
                 * Temporary Chuples function to edit the grasp_z programmatically.
                 */
                void set_grasp_z(double in_gz);

                virtual void link_info(manipulator_plant_t* manip, const util::space_t* manipulator_state_space, const util::space_t* object_state_space, const util::space_t* combined_space);

                virtual void set_transit_mode();

                virtual void set_transfer_mode();

                /**
                 * Returns true if the mode is transfer. If this function return false then we are in transit mode.
                 *
                 * @brief Checks if we are in transfer mode. If not then we are in transit mode.
                 *
                 * @return True if the mode is transfer. Otherwise false if mode is transit.
                 */
                virtual bool is_transfer_mode();

              protected:
                manipulator_plant_t* _manipulator;

                const util::space_t* manip_space;
                const util::space_t* object_space;
                const util::space_t* combined_space;

                sim::state_t* manip_point;

                util::config_t relative_configuration;

                double min_theta, max_theta;
                double grasp_z;
                int max_tries;

                unsigned gripper_mode;

            };
        }
    }
}

#endif
