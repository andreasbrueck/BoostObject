////////////////////////////////////////////////////////////////////////
//
// (C) Copyright 2015 Andreas Brück 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_OBJECT_DETAIL_SENDER_SCOPE_HPP
#define BOOST_OBJECT_DETAIL_SENDER_SCOPE_HPP

#include "config.hpp"
#include "object_internals.hpp"
#include "slot_context.hpp"

namespace boost {
namespace object {

    class object;

namespace detail {

    class sender_scope
    {
    public:
        sender_scope ( object_internals * i, const slot_context & ctx )
            :
              m_i ( i ),
              m_previous ( i->getSender () )
        {
            m_i->setSender ( ctx.signal_owner );
        }

        ~sender_scope ()
        {
            m_i->setSender ( m_previous );
        }

    private:
        BOOST_OBJECT_NONCOPYABLE(sender_scope)

    private:
        object_internals * m_i;
        object * m_previous;
    };

} // namespace detail
} // namespace object
} // namespace boost

#endif // BOOST_OBJECT_DETAIL_SENDER_SCOPE_HPP

