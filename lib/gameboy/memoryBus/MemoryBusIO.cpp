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

#include <play-man/gameboy/memoryBus/MemoryBus.hpp>
#include <play-man/gameboy/memoryBus/IORangeDefines.hpp>
#include <play-man/logger/Logger.hpp>
#include <assert.h>
#include <pthread.h>

namespace GameBoy {

    void MemoryBus::WriteIO(const uint16_t address, const uint8_t value)
    {
        assert(address >= 0xFF00 && address <= 0xFF70);

        if (address == JoyPadInputAddress)
        {
            // Writes to the JoyPad register can only have effect on the top nibble,
            // the lower nibble is ReadOnly.
            const uint8_t joyPadLowerNibble = cpu->GetCpuCore().GetJoyPadRegisterValue() & 0x0F;
            const uint8_t joyPadPossibleWriteBits = value & 0xF0;

            cpu->GetCpuCore().SetJoyPadRegister(joyPadPossibleWriteBits | joyPadLowerNibble);
        }
        else if (address >= SerialTransferAddressStart && address <= SerialTransferAddressEnd)
        {
            assert(false && "Writing to an unsupported address");
        }
        else if (address >= TimerAndDividerAddressStart && address <= TimerAndDividerAddressEnd)
        {
            assert(false && "Writing to an unsupported address");
        }
        else if (address == InterruptRequestAddress)
        {
            cpu->GetCpuCore().SetInterruptRequestRegister(value);
        }
        else if (address >= AudioAddressStart && address <= AudioAddressEnd)
        {
            assert(false && "Writing to an unsupported address");
        }
        else if (address >= AudioWavePatternAddressStart && address <= AudioWavePatternAddressEnd)
        {
            assert(false && "Writing to an unsupported address");
        }
        else if (address >= PPURegistersAddressStart && address <= PPURegistersAddressEnd)
        {
            assert(false && "Writing to an unsupported address");
        }
        else if (address == BootRomMappingControlAddress)
        {
            assert(false && "Writing to an unsupported address");
        }
        else if (address == DMGCompatibilityModeAddress)
        {
            assert(cpu->GetCpuCore().GetCgbMode() == true);
            assert(false && "Writing to an unsupported address");
        }
        else if (address == SpeedSwitchRegisterAddress)
        {
            assert(cpu->GetCpuCore().GetCgbMode() == true);
            assert(false && "Writing to an unsupported address");
        }
        else if (address == VRamBankSelectAddress)
        {
            assert(cpu->GetCpuCore().GetCgbMode() == true);
            assert(false && "Writing to an unsupported address");
        }
        else if (address >= VRamDMAAddressStart && address <= VRamDMAAddressEnd)
        {
            assert(cpu->GetCpuCore().GetCgbMode() == true);
            assert(false && "Writing to an unsupported address");
        }
        else if (address == IRPortAddress)
        {
            assert(cpu->GetCpuCore().GetCgbMode() == true);
            assert(false && "Writing to an unsupported address");
        }
        else if (address >= PPUPalettesAddressStart && address <= PPUPalettesAddressEnd)
        {
            assert(cpu->GetCpuCore().GetCgbMode() == true);
            assert(false && "Writing to an unsupported address");
        }
        else if (address == ObjectPriorityModeAddress)
        {
            assert(cpu->GetCpuCore().GetCgbMode() == true);
            assert(false && "Writing to an unsupported address");
        }
        else if (address == WorkRamBankSelect)
        {
            assert(cpu->GetCpuCore().GetCgbMode() == true);
            assert(false && "Writing to an unsupported address");
        }
        else
        {
            LOG_WARNING(MEMBUS_WRITE_OUT_OF_RANGE);
        }
    }

    uint8_t MemoryBus::ReadIO(const uint16_t address)
    {
        assert(address >= 0xFF00 && address <= 0xFF70);

        if (address == JoyPadInputAddress)
        {
            return cpu->GetCpuCore().GetJoyPadRegisterValue();
        }
        else if (address >= SerialTransferAddressStart && address <= SerialTransferAddressEnd)
        {
            assert(false && "Reading from an unsupported address");
        }
        else if (address >= TimerAndDividerAddressStart && address <= TimerAndDividerAddressEnd)
        {
            assert(false && "Reading from an unsupported address");
        }
        else if (address == InterruptRequestAddress)
        {
            return cpu->GetCpuCore().GetInterruptRequestRegisterValue();
        }
        else if (address >= AudioAddressStart && address <= AudioAddressEnd)
        {
            assert(false && "Reading from an unsupported address");
        }
        else if (address >= AudioWavePatternAddressStart && address <= AudioWavePatternAddressEnd)
        {
            assert(false && "Reading from an unsupported address");
        }
        else if (address >= PPURegistersAddressStart && address <= PPURegistersAddressEnd)
        {
            assert(false && "Reading from an unsupported address");
        }
        else if (address == BootRomMappingControlAddress)
        {
            assert(false && "Reading from an unsupported address");
        }
        else if (address == DMGCompatibilityModeAddress)
        {
            assert(cpu->GetCpuCore().GetCgbMode() == true);
            assert(false && "Reading from an unsupported address");
        }
        else if (address == SpeedSwitchRegisterAddress)
        {
            assert(cpu->GetCpuCore().GetCgbMode() == true);
            assert(false && "Reading from an unsupported address");
        }
        else if (address == VRamBankSelectAddress)
        {
            assert(cpu->GetCpuCore().GetCgbMode() == true);
            assert(false && "Reading from an unsupported address");
        }
        else if (address >= VRamDMAAddressStart && address <= VRamDMAAddressEnd)
        {
            assert(cpu->GetCpuCore().GetCgbMode() == true);
            assert(false && "Reading from an unsupported address");
        }
        else if (address == IRPortAddress)
        {
            assert(cpu->GetCpuCore().GetCgbMode() == true);
            assert(false && "Reading from an unsupported address");
        }
        else if (address >= PPUPalettesAddressStart && address <= PPUPalettesAddressEnd)
        {
            assert(cpu->GetCpuCore().GetCgbMode() == true);
            assert(false && "Reading from an unsupported address");
        }
        else if (address == ObjectPriorityModeAddress)
        {
            assert(cpu->GetCpuCore().GetCgbMode() == true);
            assert(false && "Reading from an unsupported address");
        }
        else if (address == WorkRamBankSelect)
        {
            assert(cpu->GetCpuCore().GetCgbMode() == true);
            assert(false && "Reading from an unsupported address");
        }
        else
        {
            LOG_WARNING(MEMBUS_READ_OUT_OF_RANGE);
            return OpenBusValue;
        }
    }
}
