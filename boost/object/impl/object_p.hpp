////////////////////////////////////////////////////////////////////////
//
// (C) Copyright 2015 Andreas Brück 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_OBJECT_IMPL_OBJECT_P_HPP
#define BOOST_OBJECT_IMPL_OBJECT_P_HPP

#include "../object.hpp"
#include "base_p.hpp"

#include "../detail/log.hpp"

namespace boost {
namespace object {
namespace detail {

    class object_p : public base_p
    {
        friend class boost::object::object;
        friend class application_p;

        B_OBJECT(object_p)

    protected:
        object_p ( object * o, object * parent )
            :
              base_p ( o, parent )
        {}

        void uninstall () final override
        {
            if ( m_isUninstalled )
                return;

            object * oo = static_cast < object * > ( o );

            // signal to everyone listening that this object is about to
            // be destructed
            oo->destroyed.emit ( oo );

            // predestruct the object
            if ( ! _predestruct ().val )
            {
                BOOST_OBJECT_LOG_DEBUG
                        << "Failed to delay-predestruct a BObject";

                predestruct ();
            }

            base_p::uninstall ();
        }

    private:
        object_p ( object * o, engine & e )
            :
              base_p ( o, e )
        {}

    private:
        bobj::auto_slot_t < predestruct_t () > _predestruct
            BOOST_OBJECT_BOUNDTO ( predestruct );
    };

} // namespace detail
} // namespace object
} // namespace boost

#endif // BOOST_OBJECT_IMPL_OBJECT_P_HPP

