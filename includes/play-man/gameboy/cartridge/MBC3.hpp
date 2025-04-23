// ****************************************************************************** //
//   _______   __                              __       __                        //
//  /       \ /  |                            /  \     /  |                       //
//  $$$$$$$  |$$ |  ______   __    __         $$  \   /$$ |  ______   _______     //
//  $$ |__$$ |$$ | /      \ /  |  /  | ______ $$$  \ /$$$ | /      \ /       \    //
//  $$    $$/ $$ | $$$$$$  |$$ |  $$ |/      |$$$$  /$$$$ | $$$$$$  |$$$$$$$  |   //
//  $$$$$$$/  $$ | /    $$ |$$ |  $$ |$$$$$$/ $$ $$ $$/$$ | /    $$ |$$ |  $$ |   //
//  $$ |      $$ |/$$$$$$$ |$$ \__$$ |        $$ |$$$/ $$ |/$$$$$$$ |$$ |  $$ |   //
//  $$ |      $$ |$$    $$ |$$    $$ |        $$ | $/  $$ |$$    $$ |$$ |  $$ |   //
//  $$/       $$/  $$$$$$$/  $$$$$$$ |        $$/      $$/  $$$$$$$/ $$/   $$/    //
//                          /  \__$$ |                                            //
//                          $$    $$/                                             //
//                           $$$$$$/                                              //
//                                                                                //
//                            By: K1ngmar and rvan-mee                            //
// ****************************************************************************** //

#pragma once

#include <play-man/gameboy/cartridge/ACartridge.hpp>
#include <play-man/gameboy/cartridge/CartridgeDefines.hpp>

namespace GameBoy {

/**
 * @brief RTC (Real Time Clock) register numbers
 */
#define RTC_REGISTERS_SEQ(x, n)             \
    x(n, Seconds, 0x08)          /*RTC S*/  \
    x(n, Minutes, 0x09)          /*RTC M*/  \
    x(n, Hours, 0x0A)            /*RTC H*/  \
    x(n, DaysLowerBits, 0x0B)    /*RTC DL*/ \
    x(n, DaysUpperAndFlags, 0x0C) /*RTC DH*/

CREATE_ENUM_WITH_UTILS(RTC_REGISTERS_SEQ, RTCRegisters);
#undef RTC_REGISTERS_SEQ

class MBC3Cartridge : public ACartridge
{
private:
    /*     Control Registers     */

    /**
     * @brief MBC3 cartridges support up to 128 rom banks.
     */
    uint8_t romBankNumber;

    /**
     * @brief This register either contains what is being addressed in the 0xA000 to 0xBFFF region.
     * 
     * @note If the value is between 0x00 and 0x07 the corresponding RAM bank will be accessed.
     * @note If the value is between 0x08 and 0x0C the corresponding RTC register will be accessed.
     */
    uint8_t ramOrTimerSelect;

    /**
     * @brief Writing 0x00 and then 0x01 to this register will cause the current time to become
     * 'latched' into the RTC registers. The latched data will not change until the write
     * procedure is done again.
     * 
     * This is a way to provide access to the RTC registers while the clock can keep on ticking.
     * 
     * @note The definition of latching is locking a signal in a certain state unless a
     * clock or in this case a control signal allows it to change.
     */
    uint8_t latchClockData;


    /*     RTC registers     */

    uint8_t secondsLatched;
    uint8_t minutesLatched;
    uint8_t hoursLatched;
    uint8_t daysUpperLatched;
    uint8_t daysLowerLatched;

    /**
     * @brief Writing 0x0A into the address range connected to this register will enable
     * reads and writes to both the RAM banks and the RTC registers.
     */
    bool ramAndTimerEnabled;

    /*     Misc     */

    /**
     * @brief Bool to keep track if the current cartridge has a Real Time Clock
     */
    bool hasRTC;


    /**
     * @brief Calling this function will latch the current time into the RTC registers
     */
    void LatchTimerRegisters();

public:
    MBC3Cartridge() = delete;
    MBC3Cartridge(std::unique_ptr<Rom> rom);
    ~MBC3Cartridge() = default;

    virtual uint8_t ReadByte(const uint16_t address) override;
    virtual void    WriteByte(const uint16_t address, const uint8_t value) override;
};

}
