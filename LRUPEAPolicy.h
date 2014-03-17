#ifndef LRUPEAPolicy_H
#define LRUPEAPolicy_H

#include "AbstractReplacementPolicy.h"
#include "L2Cache_PEAPriority.h"
#include <utility>

/* Simple true LRU - PEA replacement policy */

class LRUPEAPolicy : public AbstractReplacementPolicy {
 public:

  LRUPEAPolicy(Index num_sets, Index assoc);
  ~LRUPEAPolicy();

  void touch(Index set, Index way, Time time);
  Index getVictim(Index set) const;
  //void findVictims(Index set);
  pair<Index,Index> findVictims(Index set);
  Index getVictim1(Index set) const;
  Index getVictim2(Index set) const;

 private:
	Index smallest_index, next_smallest_index;
};

inline
LRUPEAPolicy::LRUPEAPolicy(Index num_sets, Index assoc)
  : AbstractReplacementPolicy(num_sets, assoc)
{  
}

inline
LRUPEAPolicy::~LRUPEAPolicy()
{
}

inline 
void LRUPEAPolicy::touch(Index set, Index index, Time time){
  assert(index >= 0 && index < m_assoc);
  assert(set >= 0 && set < m_num_sets);

  m_last_ref_ptr[set][index] = time;
}

inline
Index LRUPEAPolicy::getVictim(Index set) const {
  //  assert(m_assoc != 0);
  Time time, smallest_time;
  Index smallest_index;

  smallest_index = 0;
  smallest_time = m_last_ref_ptr[set][0];

  for (unsigned int i=0; i < m_assoc; i++) {
    time = m_last_ref_ptr[set][i];
    //assert(m_cache[cacheSet][i].m_Permission != AccessPermission_NotPresent);

    if (time < smallest_time){
      smallest_index = i;
      smallest_time = time;
    }
  }

  //  DEBUG_EXPR(CACHE_COMP, MedPrio, cacheSet);
  //  DEBUG_EXPR(CACHE_COMP, MedPrio, smallest_index);
  //  DEBUG_EXPR(CACHE_COMP, MedPrio, m_cache[cacheSet][smallest_index]);
  //  DEBUG_EXPR(CACHE_COMP, MedPrio, *this);

  return smallest_index;
}

inline
pair<Index,Index> LRUPEAPolicy::findVictims(Index set){
  //  assert(m_assoc != 0);
  Time time, smallest_time, next_smallest_time;

  if( m_last_ref_ptr[set][0] > m_last_ref_ptr[set][1]){
	  smallest_index = 1;
	  smallest_time = m_last_ref_ptr[set][1];
	  next_smallest_index = 0;
	  next_smallest_time = m_last_ref_ptr[set][0];
   }
  else {
	  smallest_index = 0;
	  smallest_time = m_last_ref_ptr[set][0];
	  next_smallest_index = 1;
	  next_smallest_time = m_last_ref_ptr[set][1];
  }

  for (unsigned int i=2; i < m_assoc; i++) {
    time = m_last_ref_ptr[set][i];
    //assert(m_cache[cacheSet][i].m_Permission != AccessPermission_NotPresent);

    if (time < smallest_time){
      next_smallest_index = smallest_index;
      next_smallest_time = smallest_time;
      smallest_index = i;
      smallest_time = time;
    }
    else if (time < next_smallest_time && time != smallest_time){
            next_smallest_index = i;
      	    next_smallest_time = time;
    }
  }

  //  DEBUG_EXPR(CACHE_COMP, MedPrio, cacheSet);
  //  DEBUG_EXPR(CACHE_COMP, MedPrio, smallest_index);
  //  DEBUG_EXPR(CACHE_COMP, MedPrio, m_cache[cacheSet][smallest_index]);
  //  DEBUG_EXPR(CACHE_COMP, MedPrio, *this);

  return make_pair(smallest_index,next_smallest_index);
}

inline
Index LRUPEAPolicy::getVictim1(Index set) const {
  return smallest_index;
}

inline
Index LRUPEAPolicy::getVictim2(Index set) const {

  return next_smallest_index;
}

#endif // PSEUDOLRUBITS_H
