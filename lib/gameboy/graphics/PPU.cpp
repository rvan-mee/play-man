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

#include <play-man/gameboy/graphics/PPU.hpp>

namespace GameBoy {

PPU::PPU(bool cgbEnabled) : CgbMode(cgbEnabled)
{
    // TODO:
    // Find the correct default values.
    (void) DMAregister;
    (void) LCDCregister;
    (void) LYregister;
    (void) LYCregister;
    (void) STATregister;
    (void) SCYregister;
    (void) SCXregister;
    (void) WYregister;
    (void) WXregister;
    (void) BGPregister;
    (void) OBP0register;
    (void) OBP1register;
    (void) BGPIregister;
    (void) BCPDregister;
    (void) OBPIregister;
    (void) OBPDregister;
    (void) state;
}

void    PPU::SetCgbMode(bool enabled)
{
    CgbMode = enabled;
}

uint8_t PPU::ReadByte(uint16_t address)
{
    (void) address;
    assert(false);
}

void    PPU::WriteByte(uint16_t address, uint8_t value)
{
    (void) address;
    (void) value;
    assert(false);
}

}
