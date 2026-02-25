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

        P1 = JoyPadNoButtonsPressed;
    }

    uint8_t	CpuCore::GetInterruptEnableRegisterValue()
    {
        return (IE);
    }

    uint8_t	CpuCore::GetInterruptRequestRegisterValue()
    {
        return (IF);
    }

    uint8_t CpuCore::GetJoyPadRegisterValue()
    {
        return (P1);
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

    void    CpuCore::SetJoyPadRegister(const uint8_t value)
    {
        std::stringstream ss;

        if ((value & 0b0011'0000) != (P1 & 0b0011'0000))
        {
            ss << "\nButton group change for:\n";
            AddButtonChangeToStringStream(ss, JoyPadButtonGroup::DIRECTION, value, P1);
            AddButtonChangeToStringStream(ss, JoyPadButtonGroup::ACTION, value, P1);
            ss << "\n";
        }

        // The lower nibble contains which buttons are pressed, this area should
        // be read only for the memory bus, any writes on these bits should be internal writes.
        if ((value & 0x0F) != (P1 & 0x0F))
        {
            ss << "Button change for\n";
            if ((value & GetEnumAsValue(JoyPadButtonGroup::DIRECTION)) == 0)
            {
                ss << "\tDirection buttons:\n";
                AddButtonChangeToStringStream(ss, JoyPadDirectionButtons::DOWN, value, P1);
                AddButtonChangeToStringStream(ss, JoyPadDirectionButtons::UP, value, P1);
                AddButtonChangeToStringStream(ss, JoyPadDirectionButtons::LEFT, value, P1);
                AddButtonChangeToStringStream(ss, JoyPadDirectionButtons::RIGHT, value, P1);
            }
            if ((value & GetEnumAsValue(JoyPadButtonGroup::ACTION)) == 0)
            {
                ss << "\tAction buttons:\n";
                AddButtonChangeToStringStream(ss, JoyPadActionButtons::START, value, P1);
                AddButtonChangeToStringStream(ss, JoyPadActionButtons::SELECT, value, P1);
                AddButtonChangeToStringStream(ss, JoyPadActionButtons::B, value, P1);
                AddButtonChangeToStringStream(ss, JoyPadActionButtons::A, value, P1);
            }
        }

        LOG_DEBUG(ss.str());
        P1 = value;
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
