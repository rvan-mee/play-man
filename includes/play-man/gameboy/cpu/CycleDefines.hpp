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

#pragma once

#include <stdint.h>

/**
 * @brief A single cycle and the smallest operation the CPU can do.
 */
constexpr uint8_t T_Tick = 1;

/**
 * @brief A 'Machine' tick, the minumum amount of T-Ticks it takes to perform an instruction.
 */
constexpr uint8_t M_Tick = 4 * T_Tick;

/**
 * @brief A 'Dot' is a time unit used for rendering (~4.194Mhz).
 */
constexpr uint8_t Dot = 1;

/**
 * @brief The amount of dots it takes to complete an OAM scan for a single scanline. 
 */
constexpr uint8_t DotsInMode2 = 80 * Dot;

/**
 * @brief The total amount of dots it takes to complete the render of a single scanline.
 */
constexpr uint16_t DotsPerScanline = 456 * Dot;

/**
 * @brief During the final scanline in the VBlank mode of the PPU, the LY register will
 * be set in the middle of that scanline, this represents the amount of dots at which
 * that happens.
 */
constexpr uint16_t DotsResetLY = DotsPerScanline / 2;

/**
 * @brief The amount of dots it takes to complete a full Vertical Blank.
 */
constexpr uint16_t DotsInMode1 = 4560 * Dot;
