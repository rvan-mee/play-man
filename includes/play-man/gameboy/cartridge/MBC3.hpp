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

    /*     RTC     */

    /**
     * @brief Real Time Clock included in some MBC3 cartridges.
     * 
     * The RTC included 2 sets of register, a set of internal register that would get updated
     * by the clock and a second set of registers that could 'latch' the internal's values into its own.
     * 
     * The way the clock emulation is done by using the device's system time, taking the difference between
     * the current time inside the UpdateInternalTime() call and the lastUpdateTime.
     * 
     * To have the correct time when trying to latch, a call to UpdateInternalTime() must be done right before.
     * 
     * To make sure that halting the clock also works we update the lastUpdateTime, before halting and un-halting.
     * When the clock is halted the internal registers will not be updated but the lastUpdateTime will be,
     * making it seem like no time has passed when the clock will be un-halted.
     * 
     * @note The Pan Docs are not the best at explaining how this chip works (imho).
     * @note The main resources I used are: https://thomas.spurden.name/gameboy/#mbc3-real-time-clock-rtc
     * @note and https://pastebin.com/7BypTSqX
     */
    class RealTimeClock
    {
    private:
        time_t  lastUpdateTime;

        /* Registers */
        uint8_t secondsInternal;
        uint8_t minutesInternal;
        uint8_t hoursInternal;
        uint8_t daysLowerInternal;
        // This register contains the upper (8th) bit of the day, a bit to check
        // if the clock is halted and a bit to check if the day counter has overflown.
        uint8_t daysUpperAndFlagsInternal;

        uint8_t secondsLatched;
        uint8_t minutesLatched;
        uint8_t hoursLatched;
        uint8_t daysLowerLatched;
        // This register contains the upper (8th) bit of the day, a bit to check
        // if the clock is halted and a bit to check if the day counter has overflown.
        uint8_t daysUpperAndFlagsLatched;

    public:
        RealTimeClock();
        ~RealTimeClock() = default;

        /**
         * @brief Updates the internal registers based on the device's system time.
         * 
         * @note Make sure to call this before anything gets latched.
         */
        void      UpdateInternalTime();
        void      LatchInternalRegisters();

        void      SetSecondsInternal(uint8_t newSeconds);
        void      SetMinutesInternal(uint8_t newMinutes);
        void      SetHoursInternal(uint8_t newHours);
        void      SetDaysLowerInternal(uint8_t newDaysLower);
        void      SetDaysUpperAndFlagsInternal(uint8_t newDaysUpperAndFlags);

        uint8_t   GetSecondsLatched() const;
        uint8_t   GetMinutesLatched() const;
        uint8_t   GetHoursLatched() const;
        uint8_t   GetDaysLowerLatched() const;
        uint8_t   GetDaysUpperAndFlagsLatched() const;
        bool      IsHalted() const;

    };

    RealTimeClock RTC;

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
     * @brief Reads the byte at the given address.
     * The selected bank can be found inside the ramOrTimerSelect register.
     * 
     * @return The byte at the address or OpenBusValue on error.
     */
    uint8_t ReadRAM(const uint16_t address);

    /**
     * @brief Reads a register inside the RTC.
     * The selected register is based on the value inside the ramOrTimerSelect register.
     * 
     * @return The byte in the register or OpenBusValue on error.
     */
    uint8_t ReadRTC();

    /**
     * @brief Writes the given value at the given address.
     * The selected bank can be found inside the ramOrTimerSelect register.
     */
    void    WriteRAM(const uint16_t address, const uint8_t value);

    /**
     * @brief Writes to an internal register inside the RTC.
     * The selected register is based on the value inside the ramOrTimerSelect register.
     * 
     */
    void    WriteRTC(const uint8_t value);

public:
    MBC3Cartridge() = delete;
    MBC3Cartridge(std::unique_ptr<Rom> rom);
    ~MBC3Cartridge() = default;

    virtual uint8_t ReadByte(const uint16_t address) override;
    virtual void    WriteByte(const uint16_t address, const uint8_t value) override;
};

}
