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

#include <play-man/gameboy/cpu/CycleDefines.hpp>
#include <play-man/gameboy/ppu/PPU.hpp>
#include <play-man/logger/Logger.hpp>

namespace GameBoy {

void    PPU::TickDMA()
{
    if (!DMATransferActive)
        return ;

    // Wait the remaining T-ticks. 
    if (CyclesRemainingDMA)
    {
        CyclesRemainingDMA--;
        return;
    }

    // Get the value at the current offset.
    const uint16_t readAddress = (DMAregister * OffsetDMATransfer) + BytesDMATransferred;
    uint8_t value = cpu->GetMemoryBus().ReadByte(readAddress);

    // Write the value to the OAM.
    oam[BytesDMATransferred] = value;
    CyclesRemainingDMA += M_Tick;

    BytesDMATransferred += 1;
    if (BytesDMATransferred > FinalDMATransferByte)
        DMATransferActive = false;
}

void    PPU::StartDmaTransfer()
{
    DMATransferActive = true;
    CyclesRemainingDMA = StartDMATransferDelay;
    BytesDMATransferred = 0x00;
}

}
