/*
* Antz
* Antz Framework
* Role.h
*
* Copyright (c) 2015 Zhi Xing. All rights reserved.
* License: <#license#>
*/


#ifndef __Antz__Role__
#define __Antz__Role__

#include <stdint.h>

namespace Antz
{
	class Role
	{
		virtual int test() = 0;
   Role();
   protected:
      virtual bool receiveSignal();

	};

}

#endif
