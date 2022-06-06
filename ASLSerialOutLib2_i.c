/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Wed Feb 09 18:07:09 2005
 */
/* Compiler settings for C:\EyeTracker5000\Windows\MSVC6\ASLSerialOutLib2\ASLSerialOutLib2.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )
#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

const IID IID_IASLSerialOutPort2 = {0x23494105,0x4C0E,0x4E25,{0xBF,0x79,0x75,0x84,0x9C,0x6F,0xB9,0xCB}};


const IID IID_IASLSerialOutPort3 = {0x7998ECCF,0xC5B4,0x4980,{0x91,0x91,0x12,0x7E,0x96,0x66,0x09,0xDF}};


const IID LIBID_ASLSERIALOUTLIB2Lib = {0x8FF83C13,0x3622,0x4CDA,{0x9E,0x8A,0x93,0x97,0x60,0x55,0x8B,0xBF}};


const IID DIID__IASLSerialOutPort2Events = {0xC8139AC3,0xA35F,0x4507,{0x91,0x8C,0x2D,0xF3,0x64,0xF8,0x8C,0x26}};


const CLSID CLSID_ASLSerialOutPort2 = {0x9A777A5F,0x2C6E,0x44C3,{0xBE,0x85,0x25,0x98,0x91,0xAC,0x82,0x10}};


const CLSID CLSID_ASLSerialOutPort3 = {0xE8697DF4,0xFEF7,0x4CD0,{0x8A,0xAD,0xF0,0xFB,0x0A,0x1A,0x33,0x2D}};


#ifdef __cplusplus
}
#endif

