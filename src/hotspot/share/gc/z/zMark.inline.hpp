/*
 * Copyright (c) 2015, 2018, Oracle and/or its affiliates. All rights reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 only, as
 * published by the Free Software Foundation.
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * version 2 for more details (a copy is included in the LICENSE file that
 * accompanied this code).
 *
 * You should have received a copy of the GNU General Public License version
 * 2 along with this work; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Please contact Oracle, 500 Oracle Parkway, Redwood Shores, CA 94065 USA
 * or visit www.oracle.com if you need additional information or have any
 * questions.
 */

#ifndef SHARE_GC_Z_ZMARK_INLINE_HPP
#define SHARE_GC_Z_ZMARK_INLINE_HPP

#include "gc/z/zAddress.inline.hpp"
#include "gc/z/zCycle.inline.hpp"
#include "gc/z/zMark.hpp"
#include "gc/z/zMarkStack.inline.hpp"
#include "gc/z/zThreadLocalData.hpp"
#include "runtime/thread.hpp"
#include "utilities/debug.hpp"

template <bool follow, bool finalizable, bool publish>
inline void ZMark::mark_object(zaddress addr) {
  assert(oopDesc::is_oop(to_oop(addr)), "Should be oop");
  ZMarkThreadLocalStacks* const stacks = ZThreadLocalData::mark_stacks(Thread::current(), _cycle->cycle_id());
  ZMarkStripe* const stripe = _stripes.stripe_for_addr(untype(addr));
  ZMarkStackEntry entry(untype(ZAddress::offset(addr)), follow, finalizable);

  assert(ZHeap::heap()->is_young(addr) == (_cycle->cycle_id() == ZCycleId::_minor), "Phase/object mismatch");

  stacks->push(allocator(), &_stripes, stripe, entry, publish);
}

inline void ZMark::mark_follow_invisible_root(zaddress addr, size_t size) {
  follow_array((uintptr_t)addr, size, false /* finalizable */);
}

#endif // SHARE_GC_Z_ZMARK_INLINE_HPP
