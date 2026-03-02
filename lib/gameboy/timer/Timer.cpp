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

#include <play-man/logger/Logger.hpp>
#include <play-man/utility/UtilFunc.hpp>

#include <play-man/gameboy/timer/Timer.hpp>
#include <play-man/gameboy/timer/TimerDefines.hpp>

namespace GameBoy {

    Timer::Timer(Cpu* _cpu) : cpu(_cpu)
    {
        divider = DividerDefaultValue;
        counter = CounterDefaultValue;
        modulo  = ModuloDefaultValue;
        control = ControlDefaultValue;

        isStopped = false;

        // TODO: To be used when timer ticks are supported.
        (void) cpu;
    }

    Timer::~Timer()
    {
    }

    void Timer::ResetDivider()
    {
        divider = DividerDefaultValue;
    }

    void Timer::SetStopped(bool stopped)
    {
        isStopped = stopped;
    }

    void Timer::WriteByte(uint16_t address, uint8_t value)
    {
        if (address == DividerRegisterAddress)
        {
            divider = 0x00;
            LOG_DEBUG("Timer: Write to the divider register, resetting to 0x00");
        }
        else if (address == CounterRegisterAddress)
        {
            counter = value;
            LOG_DEBUG("Timer: Write to the counter register, value: " + Utility::IntAsHexString(value));
        }
        else if (address == ModuloRegisterAddress)
        {
            modulo = value;
            LOG_DEBUG("Timer: Write to the modulo register, value: " + Utility::IntAsHexString(value));
        }
        else if (address == ControlRegisterAddress)
        {
            control = value;

            std::stringstream ss;
            ss << "Timer: Writing to the control register, settings: ";
            ss << static_cast<CounterState>(value & CounterEnableBit) << " - ";
            ss << static_cast<CounterClockSpeeds>(value & CounterClockBits);
            LOG_DEBUG(ss.str());
        }
        else
        {
            LOG_ERROR("Writing to an out-of-range address within the Timer");
            assert(false);
        }
    }

    uint8_t Timer::ReadByte(uint16_t address)
    {
        if (address == DividerRegisterAddress)
        {
            return divider;
        }
        else if (address == CounterRegisterAddress)
        {
            return counter;
        }
        else if (address == ModuloRegisterAddress)
        {
            return modulo;
        }
        else if (address == ControlRegisterAddress)
        {
            return control;
        }
        else
        {
            LOG_ERROR("Reading from out-of-range address within the Timer");
            assert(false);
        }
    }
}
    