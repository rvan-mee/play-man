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

#include <play-man/gameboy/cartridge/MBC3.hpp>

/*     Default Register Values     */

static constexpr uint8_t DefaultSecondsInternal = 0x00;
static constexpr uint8_t DefaultMinutesInternal = 0x00;
static constexpr uint8_t DefaultHoursInternal = 0x00;
static constexpr uint8_t DefaultDaysLowerInternal = 0x00;
static constexpr uint8_t DefaultDaysUpperAndFlagsInternal = 0x00;

static constexpr uint8_t DefaultSecondsLatched = 0x00;
static constexpr uint8_t DefaultMinutesLatched = 0x00;
static constexpr uint8_t DefaultHoursLatched = 0x00;
static constexpr uint8_t DefaultDaysUpperAndFlagsLatched = 0x00;
static constexpr uint8_t DefaultDaysLowerLatched = 0x00;

/*     Masks     */
static constexpr uint8_t HaltMask = 0b01000000;
static constexpr uint8_t DayCounterOverflowMask = 0b10000000;
static constexpr uint8_t UpperDayBitMask = 0b00000001;
static constexpr uint8_t FlagsMask = HaltMask | DayCounterOverflowMask | UpperDayBitMask;

/*     Time Values     */
static constexpr uint32_t Second = 1;
static constexpr uint32_t SecondsInMinute = 60;
static constexpr uint32_t MinutesInHour = 60;
static constexpr uint32_t HoursInDay = 24;

static constexpr uint32_t MinuteAsSeconds = Second * 60;
static constexpr uint32_t HourAsSeconds = MinuteAsSeconds * 60;
static constexpr uint32_t DayAsSeconds = HourAsSeconds * 24;

static constexpr uint32_t MaxSeconds = SecondsInMinute;
static constexpr uint32_t MaxMinutes = MinutesInHour;
static constexpr uint32_t MaxHours = HoursInDay;
static constexpr uint32_t MaxDaysLower = 0xFF;
static constexpr uint32_t MaxDaysOverflow = 512;

namespace GameBoy 
{

/*     RTC     */

MBC3Cartridge::RealTimeClock::RealTimeClock()
{
    lastUpdateTime = time(NULL);

    secondsInternal = DefaultSecondsInternal;
    minutesInternal = DefaultMinutesInternal;
    hoursInternal = DefaultHoursInternal;
    daysLowerInternal = DefaultDaysLowerInternal;
    daysUpperAndFlagsInternal = DefaultDaysUpperAndFlagsInternal;

    secondsLatched = DefaultSecondsLatched;
    minutesLatched = DefaultMinutesLatched;
    hoursLatched = DefaultHoursLatched;
    daysLowerLatched = DefaultDaysLowerLatched;
    daysUpperAndFlagsLatched = DefaultDaysUpperAndFlagsLatched;
}

// TODO: Does this have to be synced up with the CPU clock speed once we start
// adding the ability to change the clockspeed?
void MBC3Cartridge::RealTimeClock::UpdateInternalTime()
{
    time_t currentTime = time(NULL);
    time_t timeDifference;
    uint8_t carry;

    // If the clock is halted there is no new time to update to.
    // Dont try to update values if we have already done so for this timestamp.
    if (IsHalted() || currentTime <= lastUpdateTime)
    {
        lastUpdateTime = currentTime;
        return ;
    }
    lastUpdateTime = currentTime;

    // Get the difference between the UpdateTimeInternal() calls
    timeDifference = currentTime - lastUpdateTime;

    // Update Seconds
    secondsInternal += timeDifference % SecondsInMinute;
    carry = secondsInternal >= MaxSeconds;
    if (carry)
        secondsInternal -= SecondsInMinute;

    // Update Minutes
    minutesInternal += carry;
    minutesInternal += (timeDifference / MinuteAsSeconds) % MinutesInHour; 
    carry = minutesInternal >= MaxMinutes;
    if (carry)
        minutesInternal -= MaxMinutes;

    // Update Hours
    hoursInternal += carry;
    hoursInternal += (timeDifference / HourAsSeconds) % HoursInDay;
    carry = hoursInternal >= MaxHours;
    if (carry)
        hoursInternal -= MaxHours;

    // Update Days
    // Since the RTC handles days in 9 bits (by borrowing an extra bit from the next register)
    // we need something bigger than an int8 to use for the day calculations.
    uint16_t daysExtendedInt = daysLowerInternal;
    daysExtendedInt += carry;
    daysExtendedInt += (timeDifference / DayAsSeconds) | ((daysUpperAndFlagsInternal & UpperDayBitMask) << 8);
    if (daysExtendedInt >= MaxDaysOverflow)
    {
        // If we have more than 512 (MaxDaysOverflow) in the clock then we have to set the overflow bit inside the Upper register.
        daysUpperAndFlagsInternal |= DayCounterOverflowMask;
        daysExtendedInt -= MaxDaysOverflow;
    }

    if (daysExtendedInt >= MaxDaysLower)
    {
        // If the current value does not fit within the lower 8 bits use the bit inside the daysUpper register.
        daysUpperAndFlagsInternal |= UpperDayBitMask;
        daysExtendedInt -= MaxDaysLower; 
    }
    else
    {
        // If we do not have an excess of days make sure the bit is turned off
        daysUpperAndFlagsInternal &= ~UpperDayBitMask;
    }

    daysLowerInternal = daysExtendedInt & 0xFF;
}

void MBC3Cartridge::RealTimeClock::LatchInternalRegisters()
{
    UpdateInternalTime();

    secondsLatched = secondsInternal;
    minutesLatched = minutesInternal;
    hoursLatched = hoursInternal;
    daysLowerLatched = daysLowerInternal;
    daysUpperAndFlagsLatched = daysUpperAndFlagsInternal;
}

/* Setters */

void   MBC3Cartridge::RealTimeClock::SetSecondsInternal(uint8_t newSeconds)
{
    secondsInternal = newSeconds;
}

void   MBC3Cartridge::RealTimeClock::SetMinutesInternal(uint8_t newMinutes)
{
    minutesInternal = newMinutes;
}

void   MBC3Cartridge::RealTimeClock::SetHoursInternal(uint8_t newHours)
{
    hoursInternal = newHours;
}

void   MBC3Cartridge::RealTimeClock::SetDaysLowerInternal(uint8_t newDaysLower)
{
    daysLowerInternal = newDaysLower;
}

void   MBC3Cartridge::RealTimeClock::SetDaysUpperAndFlagsInternal(uint8_t newDaysUppersAndFlags)
{
    daysUpperAndFlagsInternal = newDaysUppersAndFlags & FlagsMask;
}


/* Getters */

uint8_t   MBC3Cartridge::RealTimeClock::GetSecondsLatched() const
{
    return secondsLatched;
}

uint8_t   MBC3Cartridge::RealTimeClock::GetMinutesLatched() const
{
    return minutesLatched;
}

uint8_t   MBC3Cartridge::RealTimeClock::GetHoursLatched() const
{
    return hoursLatched;
}

uint8_t   MBC3Cartridge::RealTimeClock::GetDaysLowerLatched() const
{
    return daysLowerLatched;
}

uint8_t   MBC3Cartridge::RealTimeClock::GetDaysUpperAndFlagsLatched() const
{
    return daysUpperAndFlagsLatched;
}

bool MBC3Cartridge::RealTimeClock::IsHalted() const
{
    return UpperDayBitMask & HaltMask;
}

}