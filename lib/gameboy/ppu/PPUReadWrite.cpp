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

#include <play-man/gameboy/ppu/PPU.hpp>
#include <play-man/logger/Logger.hpp>

namespace GameBoy {

uint8_t PPU::ReadByte(uint16_t address)
{
    if (address >= AddressVramStart && address <= AddressVramEnd)
    {
        // The GameBoy color has a switchable vRam bank, DMG does not.
        if (CgbMode)
        {
            return vRam[vRamBankRegister][address - AddressVramStart];
        }
        return vRam[0][address - AddressVramStart];
    }
    if (address >= AddressOamStart && address <= AddressOamEnd)
    {
        if (DMATransferActive)
        {
            LOG_DEBUG(PPU_READ_DURING_DMA);
            return OpenBusValue;
        }
        return oam[address - AddressOamStart];
    }
    switch (address)
    {
        case (AddressLCDC):
        {
            return LCDCregister; 
        }
        case (AddressSTAT):
        {
            return STATregister; 
        }
        case (AddressSCY):
        {
            return SCYregister;
        }
        case (AddressSCX):
        {
            return SCXregister;
        }
        case (AddressLY):
        {
            return LYregister;
        }
        case (AddressLYC):
        {
            return LYCregister;
        }
        case (AddressDMA):
        {
            return DMAregister;
        }
        case (AddressBGP):
        {
            if (CgbMode)
            {
                LOG_DEBUG(PPU_READ_IN_CGB_MODE);
                return OpenBusValue;
            }

            return BGPregister;
        }
        case (AddressOBP0):
        {
            if (CgbMode)
            {
                LOG_DEBUG(PPU_READ_IN_CGB_MODE);
                return OpenBusValue;
            }

            return OBP0register;
        }
        case (AddressOBP1):
        {
            if (CgbMode)
            {
                LOG_DEBUG(PPU_READ_IN_CGB_MODE);
                return OpenBusValue;
            }

            return OBP1register;
        }
        case (AddressWY):
        {
            return WYregister;
        }
        case (AddressWX):
        {
            return WXregister;
        }
        case (AddressVramBank):
        {
            if (!CgbMode)
            {
                LOG_DEBUG(PPU_READ_IN_NON_CGB_MODE);
                return OpenBusValue;
            }

            return vRamBankRegister;
        }
        case (AddressBGPI):
        {
            if (!CgbMode)
            {
                LOG_DEBUG(PPU_READ_IN_NON_CGB_MODE);
                return OpenBusValue;
            }

            return BGPIregister;
        }
        case (AddressBCPD):
        {
            if (!CgbMode)
            {
                LOG_DEBUG(PPU_READ_IN_NON_CGB_MODE);
                return OpenBusValue;
            }

            return BCPDregister;
        }
        case (AddressOBPI):
        {
            if (!CgbMode)
            {
                LOG_DEBUG(PPU_READ_IN_NON_CGB_MODE);
                return OpenBusValue;
            }

            if (state == PixelProcessingState::Drawing)
            {
                LOG_DEBUG(PPU_READ_IN_MODE_3);
                return OpenBusValue;
            }

            return OBPDregister;
        }
        case (AddressOBPD):
        {
            if (!CgbMode)
            {
                LOG_DEBUG(PPU_READ_IN_NON_CGB_MODE);
                return OpenBusValue;
            }

            return OBPDregister;
        }
        default:
        {
            LOG_WARNING(PPU_READ_OUT_OF_RANGE);
            return OpenBusValue;
        }
    }
}

void    PPU::WriteByte(uint16_t address, uint8_t value)
{
    if (address >= AddressVramStart && address <= AddressVramEnd)
    {
        // The GameBoy color has a switchable vRam bank, DMG does not.
        if (CgbMode)
        {
            vRam[vRamBankRegister][address - AddressVramStart] = value;
            return ;
        }
        vRam[0][address - AddressVramStart] = value;
        return ;
    }
    if (address >= AddressOamStart && address <= AddressOamEnd)
    {
        if (DMATransferActive)
        {
            LOG_DEBUG(PPU_WRITE_DURING_DMA);
            return;
        }
        oam[address - AddressOamStart] = value;
        return;
    }
    switch (address)
    {
        case (AddressLCDC):
        {
            LCDCregister = value; 
            break;
        }
        case (AddressSTAT):
        {
            const uint8_t ppuModeBits = (STATregister & PPUModeMask);
            STATregister = (value & WriteMaskSTATvalue) | ppuModeBits; 
            break;
        }
        case (AddressSCY):
        {
            SCYregister = value;
            break;
        }
        case (AddressSCX):
        {
            SCXregister = value;
            break;
        }
        case (AddressLYC):
        {
            LYCregister = value;
            CompareLYC();
            break;
        }
        case (AddressDMA):
        {
            // The value stored inside the register is the address the DMA should start from, divided by 0x100.
            // Writing to this register will start the DMA transfer.
            DMAregister = value;
            StartDmaTransfer();
            break;
        }
        case (AddressBGP):
        {
            if (CgbMode)
            {
                LOG_DEBUG(PPU_WRITE_IN_CGB_MODE);
                break;
            }

            BGPregister = value;
            break;
        }
        case (AddressOBP0):
        {
            if (CgbMode)
            {
                LOG_DEBUG(PPU_WRITE_IN_CGB_MODE);
                break;
            }

            OBP0register = value;
            break;
        }
        case (AddressOBP1):
        {
            if (CgbMode)
            {
                LOG_DEBUG(PPU_WRITE_IN_CGB_MODE);
                break;
            }

            OBP1register = value;
            break;
        }
        case (AddressWY):
        {
            WYregister = value;
            break;
        }
        case (AddressWX):
        {
            WXregister = value;
            break;
        }
        case (AddressVramBank):
        {
            if (!CgbMode)
            {
                LOG_DEBUG(PPU_WRITE_IN_NON_CGB_MODE);
                break;
            }

            vRamBankRegister = value & vRamBankMask;
            break;
        }
        case (AddressBGPI):
        {
            if (!CgbMode)
            {
                LOG_DEBUG(PPU_WRITE_IN_NON_CGB_MODE);
                break;
            }

            BGPIregister = value;
            break;
        }
        case (AddressBCPD):
        {
            if (!CgbMode)
            {
                LOG_DEBUG(PPU_WRITE_IN_NON_CGB_MODE);
                break;
            }

            BCPDregister = value;
            break;
        }
        case (AddressOBPI):
        {
            if (!CgbMode)
            {
                LOG_DEBUG(PPU_WRITE_IN_NON_CGB_MODE);
                break;
            }

            if (state == PixelProcessingState::Drawing)
            {
                LOG_DEBUG(PPU_WRITE_IN_MODE_3);
                break;
            }

            OBPDregister = value;
            break;
        }
        case (AddressOBPD):
        {
            if (!CgbMode)
            {
                LOG_DEBUG(PPU_WRITE_IN_NON_CGB_MODE);
                break;
            }

            OBPDregister = value;
            break;
        }
        default:
        {
            LOG_WARNING(PPU_WRITE_OUT_OF_RANGE);
            break;
        }
    }
}

}
