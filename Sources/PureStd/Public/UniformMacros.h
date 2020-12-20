#pragma once

#define CONCAT_MACRO_TWO_PARAMS_(x,y) x##y
#define CONCAT_MACRO_TWO_PARAMS(x,y) CONCAT_MACRO_TWO_PARAMS_(x,y)
#define CONCAT_MACRO_THREE_PARAMS_(x,y,z) x##y##z
#define CONCAT_MACRO_THREE_PARAMS(x,y,z) CONCAT_MACRO_THREE_PARAMS_(x,y,z)
#define CONCAT_MACRO_FOUR_PARAMS_(w,x,y,z) w##x##y##z
#define CONCAT_MACRO_FOUR_PARAMS(w,x,y,z) CONCAT_MACRO_FOUR_PARAMS_(w,x,y,z)
#define CONCAT_MACRO_FIVE_PARAMS_(v,w,x,y,z) v##w##x##y##z
#define CONCAT_MACRO_FIVE_PARAMS(v,w,x,y,z) CONCAT_MACRO_FIVE_PARAMS_(v,w,x,y,z)
#define CONCAT_MACRO_SIX_PARAMS_(u,v,w,x,y,z) v##w##x##y##z
#define CONCAT_MACRO_SIX_PARAMS(u,v,w,x,y,z) CONCAT_MACRO_SIX_PARAMS_(u,v,w,x,y,z)
