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

#include <play-man/gameboy/cpu/CpuCore.hpp>
#include <play-man/utility/UtilFunc.hpp>
#include <play-man/logger/Logger.hpp>

namespace GameBoy
{
    void CpuCore::ClearRegisters()
    {
        AF = 0x00'00;
        BC = 0x00'00;
        DE = 0x00'00;
        HL = 0x00'00;
        SP = 0x00'00;
        PC = 0x00'00;
        IE = 0x00;

        IF = 0x00;
        IME = false;
        stateIME = InterruptState::NONE;
    }

    uint8_t	CpuCore::GetInterruptEnableRegisterValue()
    {
        return (IE);
    }

    uint8_t	CpuCore::GetInterruptRequestRegisterValue()
    {
        return (IF);
    }

    void	CpuCore::SetInterruptEnableRegister(const uint8_t value)
    {
        std::stringstream ss;

        ss << "Enabling interrupts for: ";
        if (value & GetEnumAsValue(InterruptFlags::JOYPAD))
            ss << InterruptFlags::JOYPAD << " ";
        if (value & GetEnumAsValue(InterruptFlags::LCD))
            ss << InterruptFlags::LCD << " ";
        if (value & GetEnumAsValue(InterruptFlags::SERIAL))
                ss << InterruptFlags::SERIAL << " ";
        if (value & GetEnumAsValue(InterruptFlags::TIMER))
            ss << InterruptFlags::TIMER << " ";
        if (value & GetEnumAsValue(InterruptFlags::VBLANK))
            ss << InterruptFlags::VBLANK << " ";
        if (value)
            LOG_DEBUG(ss.str());
        if (!value)
            LOG_DEBUG("Clearing enabled interrupts");

        IE = value;
    }

    void	CpuCore::SetInterruptRequestRegister(const uint8_t value)
    {
        std::stringstream ss;

        ss << "Requesting interrupts for: ";
        if (value & GetEnumAsValue(InterruptFlags::JOYPAD))
            ss << InterruptFlags::JOYPAD << " ";
        if (value & GetEnumAsValue(InterruptFlags::LCD))
            ss << InterruptFlags::LCD << " ";
        if (value & GetEnumAsValue(InterruptFlags::SERIAL))
                ss << InterruptFlags::SERIAL << " ";
        if (value & GetEnumAsValue(InterruptFlags::TIMER))
            ss << InterruptFlags::TIMER << " ";
        if (value & GetEnumAsValue(InterruptFlags::VBLANK))
            ss << InterruptFlags::VBLANK << " ";
        if (value)
            LOG_DEBUG(ss.str());
        if (!value)
            LOG_DEBUG("Clearing requested interrupts");

        IF = value;
    }

    void CpuCore::SetFlag(FlagRegisterFlag flag, bool enable)
    {
        if (enable)
        {
            AF.SetLowByte(AF.LowByte() | static_cast<uint8_t>(flag));
        }
        else
        {
            AF.SetLowByte(AF.LowByte() & ~(static_cast<uint8_t>(flag)));
        }
    }

    bool CpuCore::GetFlag(FlagRegisterFlag flag)
    {
        return (AF.LowByte() & static_cast<uint8_t>(flag)) > 0;
    }

    bool CpuCore::GetCgbMode() const
    {
        return cgbMode;
    }

    uint16_t CpuCore::GetStackPointerInc()
    {
        const uint16_t value = SP.Value();

        SP++;
        return value;
    }

    uint16_t CpuCore::GetStackPointerDec()
    {
        SP--;
        return SP.Value();
    }

    /*           ostream overload           */

    std::ostream& operator << (std::ostream& lhs, const CpuCore& core)
    {
        lhs << "Register:       Value:\n";
        lhs << "AF              " << Utility::IntAsHexString(core.AF.HighByte()) << " " << Utility::IntAsHexString(core.AF.LowByte());
        
        // Print the bits of the flag register
        lhs << " Flags: 0b";
        for (int8_t i = 7; i >= 0; i--)
        {
            lhs << (((core.AF.LowByte() >> i) & 1) ? "1" : "0");
        }
        lhs << "\n";

        lhs << "BC              " << Utility::IntAsHexString(core.BC.HighByte()) << " " << Utility::IntAsHexString(core.BC.LowByte()) << "\n";
        lhs << "DE              " << Utility::IntAsHexString(core.DE.HighByte()) << " " << Utility::IntAsHexString(core.DE.LowByte()) << "\n";
        lhs << "HL              " << Utility::IntAsHexString(core.HL.HighByte()) << " " << Utility::IntAsHexString(core.HL.LowByte()) << "\n";
        lhs << "SP              " << Utility::IntAsHexString(core.SP.Value()) << "\n";
        lhs << "PC              " << Utility::IntAsHexString(core.PC.Value()) << "\n";
        lhs << "IE              " << Utility::IntAsHexString(core.IE) << "\n";
        return lhs;
    }

} /* namespace Gameboy */
