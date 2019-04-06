/** 
 * @file zz_channel_xy.h
 * @brief 2D-channel class.
 * @author Jiho Choi (zho@korea.com)
 * @version 1.0
 * @date    12-feb-2003
 *
 * $Header: /engine/include/zz_channel_xy.h 2     04-07-07 3:29a Zho $
 */

#ifndef	__ZZ_CHANNEL_XY_H__
#define __ZZ_CHANNEL_XY_H__

#include <vector>

#ifndef __ZZ_CHANNEL_H__
#include "zz_channel.h"
#endif

class zz_motion;

//--------------------------------------------------------------------------------
class zz_channel_xy : public zz_channel {
  friend class zz_motion;

protected:
  vec2*        xys;
  unsigned int num_xys;

public:
          zz_channel_xy(void );
  virtual ~zz_channel_xy(void);

  // vector related
  void assign(int size) override;
  void clear(void ) override;
  int  size(void  ) override;

  // set methods
  void get_by_frame(int    frame, void* data_pointer) override;
  void get_by_time(zz_time time, int    fps, void* data_pointer) override;

  // set methods
  void set_by_frame(int frame, void* data_pointer) override;
};

#endif // __ZZ_CHANNEL_XY_H__
