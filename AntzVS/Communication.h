/*
 * Antz
 * Antz Framework
 * Communication.h
 * 	This file defines a protocol which is used in the communication between robots
 * 
 * Copyright (c) 2014 Zhi Xing. All rights reserved.
 * License: <#license#>
 */

#ifndef __Antz__Communication__
#define __Antz__Communication__

/* number of bits in a signal */
#define NUM_BITS        32

/* signal length in microseconds */
#define LEN_SIGN        1200	// signature of the signal
#define LEN_ONE         600	// length for sending a 1
#define LEN_ZERO        300	// length for sending a 0
#define LEN_INTR        300	// interval between two bits

/*
 * a signal adhering to the above protocal can be visualized as:
 *
 *   SIGN   1    0   0   1    0
 *   ____   __   _   _   __   _
 * _|    |_|  |_| |_| |_|  |_| |_
 * I      I    I   I   I    I   I	I --> interval
 */

/* preservation time for received data */
#define LEN_PRSV        ((uint32_t)(LEN_INTR + LEN_ZERO) * NUM_BITS)	// the received data needs to be preserved before it is read and interpreted

#endif
