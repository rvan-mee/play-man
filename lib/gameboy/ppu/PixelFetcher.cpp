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

#include <play-man/gameboy/ppu/PixelFetcher.hpp>

namespace GameBoy {

	
PixelFetcher::PixelFetcher(PPU* _ppu) : backgroundFiFo(_ppu), objectFiFo(_ppu), ppu(_ppu)
{
}

void PixelFetcher::Reset()
{
	backgroundFiFo.Clear();
	objectFiFo.Clear();
	fetcherX = 0;
	mixerX = 0;
}

void PixelFetcher::Tick()
{
	backgroundFiFo.TickFetcher();
	objectFiFo.TickFetcher();
	PixelMixerTick();
}

bool PixelFetcher::DoneWithScanline()
{
	return mixerX >= PixelsPerScanline;
}

}
