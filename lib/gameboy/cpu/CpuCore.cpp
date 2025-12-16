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
    }

    uint8_t	CpuCore::GetInterruptRegister()
    {
        return (IE);
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

    /*           ostream overload           */

    std::ostream& operator << (std::ostream& lhs, const CpuCore& core)
    {
        lhs << "Register:       Value:\n";
        lhs << "AF              " << Utility::IntAsHexString(core.AF.HighByte()) << " ";
        // Print the bits of the flag register
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
