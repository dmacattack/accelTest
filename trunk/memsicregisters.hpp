#ifndef MEMSICREGISTERS_HPP
#define MEMSICREGISTERS_HPP

#include <QString>

namespace MEMSIC
{

    // registers
    enum eREGISTERS
    {
        eREG_EXT_STATUS1 = 0X00,
        eREG_EXT_STATUS2 = 0X01,
        eREG_XOUT_LSB    = 0X02,
        eREG_XOUT_MSB    = 0X03,
        eREG_YOUT_LSB    = 0X04,
        eREG_YOUT_MSB    = 0X05,
        eREG_ZOUT_LSB    = 0X06,
        eREG_ZOUT_MSB    = 0X07,
        eREG_STATUS1     = 0X08,
        eREG_STATUS2     = 0X09,
        eREG_FEATURE1    = 0X0D,
        eREG_FEATURE2    = 0X0E,
        eREG_INIT1       = 0X0F,
        eREG_MODEC       = 0X10,
        eREG_RATE1       = 0X11,
        eREG_SNIFF_C     = 0X12,
        eREG_SNIFFTH_C   = 0X13,
        eREG_SNIFFCF_C   = 0X14,
        eREG_RANGE_C     = 0X15,
        eREG_FIFO_C      = 0X16,
        eREG_INTR_C      = 0X17,
        eREG_INIT3       = 0X1A,
        eREG_SCRATCH     = 0X1B,
        eREG_PMCR        = 0X1C,
        eREG_DMX         = 0X20,
        eREG_DMY         = 0X21,
        eREG_DMZ         = 0X22,
        eREG_RESET       = 0X24,
        eREG_INIT2       = 0X28,
        eREG_TRIGC       = 0X29,
        eREG_XOFFL       = 0X2A,
        eREG_XOFFH       = 0X2B,
        eREG_YOFFl       = 0X2C,
        eREG_YOFFH       = 0X2D,
        eREG_ZOFFL       = 0X2E,
        eREG_ZOFFH       = 0X2F,
        eREG_XGAIN       = 0X30,
        eREG_YGAIN       = 0X31,
        eREG_ZGAIN       = 0X32,
    };

    // modes defined in register 0x08 & 0x10
    enum eACCELEROMETER_MODE
    {
        eMODE_SLEEP   = 0x00,
        eMODE_STANDBY = 0x01,
        eMODE_SNIFF   = 0x02,
        eMODE_CWAKE   = 0x05,
        eMODE_SWAKE   = 0x06,
        eMODE_TRIG    = 0x07
    };

    // the resolutions defined in register 0x15
    enum eACCELEROMETER_RESOLUTION
    {
        eRES_6BITS  = 0x00,
        eRES_7BITS  = 0x01,
        eRES_8BITS  = 0x02,
        eRES_10BITS = 0x03,
        eRES_12BITS = 0x04,
        eRES_14BITS = 0x05 // requires FIFO
    };

    // the ranges defined in register 0x15
    enum eACCELEROMETER_RANGE
    {
        eRANGE_2G  = 0x00, // +/- 2g
        eRANGE_4G  = 0x01, // +/- 4g
        eRANGE_8G  = 0x02, // +/- 8g
        eRANGE_16G = 0x03, // +/- 16g
        eRANGE_12G = 0x04  // +/- 12g
    };

    // extended status register 1 (0x00)
    union tRegExtendedStatus1
    {
        unsigned char value;
        struct
        {
            char resvd     : 3; // b0:2
            char i2cAD0Bit : 1; // b3
            char resvd2    : 4; // b4:7
        };

        // should be printed with qDebug().noquote()
        QString toString()
        {
            return QString("register: = 0x%1 \n"
                           "   i2c_ad0_bit : %2")
                    .arg(value, 1, 16)
                    .arg(i2cAD0Bit == 0 ? "7bit addr = 0x4C" : "7bit addr = 0x6C");
        }
    };

    // extended status register 2 (0x01)
    union tRegExtendedStatus2
    {
        unsigned char value;
        struct
        {
            char ovrData     : 1; // b0
            char pdClockStat : 1; // b1
            char resvd       : 2; // b2:4
            char otpBusy     : 1; // b5
            char sniffEn     : 1; // b6
            char sniffDet    : 1; // b7
        };

        // should be printed with qDebug().noquote()
        QString toString()
        {
            return QString("register: = 0x%1 \n"
                           "   ovr_data     : %2 \n"
                           "   pd_clk_stat  : %3 \n"
                           "   otp_busy     : %4 \n"
                           "   sniff_en     : %5 \n"
                           "   sniff_detect : %6 ")
                    .arg(value, 1, 16)
                    .arg(ovrData)
                    .arg(pdClockStat)
                    .arg(otpBusy)
                    .arg(sniffEn)
                    .arg(sniffDet);
        }
    };

    // x-axis/y-axis/z-axis registers
    // (x = 0x02 & 0x03)
    // (y = 0x04 & 0x05)
    // (z = 0x06 & 0x07)
    union tRegAxis
    {
        short value;
        struct
        {
            unsigned char lsb : 8;
            unsigned char msb : 8;
        };
    };

    // status register 1 (0x08)
    union tRegStatusRegister1
    {
        unsigned char value;
        struct
        {
            unsigned char mode       : 3; // b0:2
            unsigned char newData    : 1; // b3
            unsigned char fifoEmpty  : 1; // b4
            unsigned char fifoFull   : 1; // b5
            unsigned char fifoThresh : 1; // b6
            unsigned char intPend    : 1; // b7
        };

        // should be printed with qDebug().noquote()
        QString toString()
        {
            QString modeStr = (mode == eMODE_SLEEP   ? "sleep" :
                              (mode == eMODE_STANDBY ? "standby" :
                              (mode == eMODE_SNIFF   ? "sniff" :
                              (mode == eMODE_CWAKE   ? "cwake" :
                              (mode == eMODE_SWAKE   ? "swake" :
                              (mode == eMODE_TRIG    ? "trig" : "resv" ))))));

            return QString("register: = 0x%1 \n"
                           "   mode        : %2 \n"
                           "   new_data    : %3 \n"
                           "   fifo_empty  : %4 \n"
                           "   fifo_full   : %5 \n"
                           "   fifo_thresh : %6 \n"
                           "   int_pend    : %7 ")
                    .arg(value, 1, 16)
                    .arg(modeStr)
                    .arg(newData)
                    .arg(fifoEmpty)
                    .arg(fifoFull)
                    .arg(fifoThresh)
                    .arg(intPend);
        }
    };

    // status register 2 (0x09)
    union tRegStatusRegister2
    {
        unsigned char value;
        struct
        {
            unsigned char resvd         : 2; // b0:1
            unsigned char intWake       : 1; // b2
            unsigned char intAcq        : 1; // b3
            unsigned char intFifoEmpty  : 1; // b4
            unsigned char intFifoFull   : 1; // b5
            unsigned char intFifoThresh : 1; // b6
            unsigned char intSWake      : 1; // b7
        };

        // should be printed with qDebug().noquote()
        QString toString()
        {
            return QString("register: = 0x%1 \n"
                           "   int_wake        : %2 \n"
                           "   int_acq         : %3 \n"
                           "   int_fifo_empty  : %4 \n"
                           "   int_fifo_full   : %5 \n"
                           "   int_fifo_thresh : %6 \n"
                           "   int_swake       : %7 ")
                    .arg(value, 1, 16)
                    .arg(intWake)
                    .arg(intAcq)
                    .arg(intFifoEmpty)
                    .arg(intFifoFull)
                    .arg(intFifoThresh)
                    .arg(intSWake);
        }
    };

    // feature register 2 (0x0D)
    union tRegFeatureRegister1
    {
        unsigned char value;
        struct
        {
            unsigned char alwaysZero : 3; // b0:2
            unsigned char freeze     : 1; // b3
            unsigned char intScEn    : 1; // b4
            unsigned char spi3En     : 1; // b5
            unsigned char i2cEn      : 1; // b6
            unsigned char spiEn      : 1; // b7
        };

        // ctor
        tRegFeatureRegister1()
        {
            value = 0x00;
        }

        // should be printed with qDebug().noquote()
        QString toString()
        {
            return QString("register: = 0x%1 \n"
                           "   freeze   : %2 \n"
                           "   intsc_en : %3 \n"
                           "   spi3_en  : %4 \n"
                           "   i2c_en   : %5 \n"
                           "   spi_en   : %6 ")
                    .arg(value, 1, 16)
                    .arg(freeze)
                    .arg(intScEn)
                    .arg(spi3En)
                    .arg(i2cEn)
                    .arg(spiEn);
        }
    };

    // feature register 2 (0x0E)
    union tRegFeatureRegister2
    {
        unsigned char value;
        struct
        {
            unsigned char wrapA        : 1; // b0
            unsigned char fifoBurst    : 1; // b1
            unsigned char spiStatEn    : 1; // b2
            unsigned char fifoStatEn   : 1; // b3
            unsigned char i2cIntClr    : 1; // b4
            unsigned char fifoStream   : 1; // b5
            unsigned char extTrigPol   : 1; // b6
            unsigned char extTrigEn    : 1; // b7
        };

        // should be printed with qDebug().noquote()
        QString toString()
        {
            return QString("register: = 0x%1 \n"
                           "   wrapa         : %2 \n"
                           "   fifo_burst    : %3 \n"
                           "   spi_stat_en   : %4 \n"
                           "   fifo_stat_en  : %5 \n"
                           "   i2cint_wrclre : %6 \n"
                           "   fifo_stream   : %7 \n"
                           "   ext_trig_pol  : %8 \n"
                           "   ext_trig_en   : %9 ")
                    .arg(value, 1, 16)
                    .arg(wrapA)
                    .arg(fifoBurst)
                    .arg(spiStatEn)
                    .arg(fifoStatEn)
                    .arg(extTrigPol)
                    .arg(extTrigEn);
        }
    };

    // init register 1 (0x0F) - bitvector N/A

    // mode control register 2 (0x10)
    union tRegModeControl
    {
        unsigned char value;
        struct
        {
            unsigned char mode      : 3; // b0:2
            unsigned char resvd     : 1; // b3
            unsigned char xAxisEn   : 1; // b4
            unsigned char yAxisEn   : 1; // b5
            unsigned char zAxisEn   : 1; // b6
            unsigned char trigCmd   : 1; // b7
        };

        // should be printed with qDebug().noquote()
        QString toString()
        {
            QString modeStr = (mode == eMODE_SLEEP   ? "sleep" :
                              (mode == eMODE_STANDBY ? "standby" :
                              (mode == eMODE_SNIFF   ? "sniff" :
                              (mode == eMODE_CWAKE   ? "cwake" :
                              (mode == eMODE_SWAKE   ? "swake" :
                              (mode == eMODE_TRIG    ? "trig" : "resv" ))))));

            return QString("register: = 0x%1 \n"
                           "   mode       : %2 \n"
                           "   x_axis_en  : %3 \n"
                           "   y_axis_en  : %4 \n"
                           "   z_axis_en  : %5 \n"
                           "   trig_cmd   : %6 ")
                    .arg(value, 1, 16)
                    .arg(modeStr)
                    .arg(xAxisEn)
                    .arg(yAxisEn)
                    .arg(zAxisEn)
                    .arg(trigCmd);
        }
    };

    // TODO 0x11 -> 0x14

    // mode control register 2 (0x15)
    union tRegRangeControl
    {
        unsigned char value;
        struct
        {
            unsigned char resolution : 3; // b0:2
            unsigned char resvd      : 1; // b3
            unsigned char range      : 1; // b4:6
            unsigned char resvd2     : 1; // b7
        };

        // should be printed with qDebug().noquote()
        QString toString()
        {
            QString resStr = (resolution == eRES_6BITS  ? "6bit"  :
                             (resolution == eRES_7BITS  ? "7bit"  :
                             (resolution == eRES_8BITS  ? "8bit"  :
                             (resolution == eRES_10BITS ? "10bit" :
                             (resolution == eRES_12BITS ? "12bit" :
                             (resolution == eRES_14BITS ? "14bit" : "" ))))));

            QString rangStr = (range == static_cast<unsigned char>(eRANGE_2G ) ? "+/- 2g" :
                              (range == static_cast<unsigned char>(eRANGE_4G ) ? "+/- 4g" :
                              (range == static_cast<unsigned char>(eRANGE_8G ) ? "+/- 8g" :
                              (range == static_cast<unsigned char>(eRANGE_16G) ? "+/- 16g" :
                              (range == static_cast<unsigned char>(eRANGE_12G) ? "+/- 12g" : "" )))));
            return QString("register: = 0x%1 \n"
                           "   resolution : %2 \n"
                           "   range      : %3 ")
                    .arg(value, 1, 16)
                    .arg(resStr)
                    .arg(rangStr);
        }
    };

    // init register 3 (0x1A) - bitvector N/A

    // scratch pad register (0x1B) - bitvector N/A


    // reset register (0x24)
    union tRegResetRegister
    {
        unsigned char value;
        struct
        {
            unsigned char resvd    : 1; // b0:5
            unsigned char reset    : 1; // b6
            unsigned char reload   : 1; // b7
        };
    };

    // init register 2 (0x28) - bitvector N/A

    // trigger count register (0x29) - bitvector N/A

    // x-axis/y-axis/z-axis offset registers
    // (x = 0x2A & 0x2B)
    // (y = 0x2C & 0x2D)
    // (z = 0x2E & 0x2F)
    // TODO contains 2s complement so it needs converting
    union tRegAxisOffset
    {
        short value;
        struct
        {
            unsigned char offL : 8;
            unsigned char offH : 8; // just upper bits
        };
    };

    // x-axis/y-axis/z-axis gain registers
    // (x = 0x2B & 0x30)
    // (y = 0x2D & 0x31)
    // (z = 0x2F & 0x32)
    // TODO this is part of the offset register
    union tByte
    {
        unsigned char value;

        struct
        {
            unsigned char b0;
            unsigned char b1;
            unsigned char b2;
            unsigned char b3;
            unsigned char b4;
            unsigned char b5;
            unsigned char b6;
            unsigned char b7;
        };
        tByte()
        {
            value = 0x00;
        }

        tByte(unsigned char val)
        {
            value = val;
        }
    };

    struct tRegAxisGain
    {
        unsigned char gain;
        unsigned char offH;

        // only use the MSB of offH
        unsigned char getGain()
        {
            return (gain | ((offH & 0x80) << 8));
        }
    };




}


#endif // MEMSICREGISTERS_HPP
