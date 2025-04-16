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

#include <cstddef>
#include "play-man/utility/EnumMacro.hpp"

constexpr size_t nintendoLogoSize = 48;

// Parts of the title are used for the manufacturer code and cgb flag on later cartridges, reducing the actual title size.
constexpr size_t romTitleSize = 16;
constexpr size_t manufacturerCodeSize = 4;

constexpr size_t nintendoLogoIndex = 0x0104;
constexpr size_t titleIndex = 0x0134;
constexpr size_t manufacturerCodeIndex = 0x013F;
constexpr size_t cgbFlagIndex = 0x0143;
constexpr size_t newLicensingCodeIndex = 0x0144;
constexpr size_t sgbFlagIndex = 0x0146;
constexpr size_t cartridgeTypeIndex = 0x0147;
constexpr size_t romSizeIndex = 0x0148;
constexpr size_t ramSizeIndex = 0x0149;
constexpr size_t destinationCodeindex = 0x014A;
constexpr size_t oldLicensingCodeindex = 0x014B;
constexpr size_t romVersionIndex =  0x014C;
constexpr size_t headerChecksumIndex = 0x014D;
constexpr size_t globalChecksumIndex = 0x014E; 
constexpr size_t romHeaderSize = 0x14F;

#define CgbFlagSeq(x, n) \
    x(n, NotSet, 0x00)              \
    x(n, BackwardsCompatible, 0x80) \
    x(n, CgbOnly, 0xC0)

CREATE_ENUM_WITH_UTILS(CgbFlagSeq, CgbFlag);
#undef CgbFlagSeq

/**
 * @brief These are the new licensing codes inside the header of a ROM.
 * They are encoded as 2-byte values, representing ascii characters.
 * 
 * @note If we take Nintendo's R&D licensing code as an example, it is encoded as the ascii values 0 and 1.
 * If we translate these ascii values to decimal values we end up with 48 and 49.
 * Then translating those to hex we get 0x30 and 0x31.
 * 
 * The final encoding will be 0x3031, we have separated the two bytes with a \' to make reading the values easier.
 */
#define NewLicensingCodeSeq(x, n) \
    x(n, UseOldLicensingCode                     , 0x00'00) \
    x(n, None                                    , 0x30'30)  /* "00" */ \
    x(n, NintendoResearchAndDevelopment          , 0x30'31)  /* "01" */ \
    x(n, Capcom                                  , 0x30'38)  /* "08" */ \
    x(n, ElectronicArts                          , 0x31'33)  /* "13" */ \
    x(n, HudsonSoft                              , 0x31'38)  /* "18" */ \
    x(n, bAI                                     , 0x31'39)  /* "19" */ \
    x(n, KSS                                     , 0x32'30)  /* "20" */ \
    x(n, PlanningOfficeWADA                      , 0x32'32)  /* "22" */ \
    x(n, PCM_Complete                            , 0x32'34)  /* "24" */ \
    x(n, SanX                                    , 0x32'35)  /* "25" */ \
    x(n, Kemco                                   , 0x32'38)  /* "28" */ \
    x(n, SETACorporation                         , 0x32'39)  /* "29" */ \
    x(n, Viacom                                  , 0x33'30)  /* "30" */ \
    x(n, Nintendo                                , 0x33'31)  /* "31" */ \
    x(n, Bandai                                  , 0x33'32)  /* "32" */ \
    x(n, OceanSoftwareAcclaimEntertainment       , 0x33'33)  /* "33" */ \
    x(n, Konami                                  , 0x33'34)  /* "34" */ \
    x(n, HectorSoft                              , 0x33'35)  /* "35" */ \
    x(n, Taito                                   , 0x33'37)  /* "37" */ \
    x(n, HudsonSoft_2                            , 0x33'38)  /* "38" */ \
    x(n, Banpresto                               , 0x33'39)  /* "39" */ \
    x(n, UbiSoft                                 , 0x34'31)  /* "41" */ \
    x(n, Atlus                                   , 0x34'32)  /* "42" */ \
    x(n, MalibuInteractive                       , 0x34'34)  /* "44" */ \
    x(n, Angel                                   , 0x34'36)  /* "46" */ \
    x(n, BulletProofSoftware                     , 0x34'37)  /* "47" */ \
    x(n, Irem                                    , 0x34'39)  /* "49" */ \
    x(n, Absolute                                , 0x35'30)  /* "50" */ \
    x(n, AcclaimEntertainment                    , 0x35'31)  /* "51" */ \
    x(n, Activision                              , 0x35'32)  /* "52" */ \
    x(n, SammyUSACorporation                     , 0x35'33)  /* "53" */ \
    x(n, HiTechExpressions                       , 0x35'35)  /* "55" */ \
    x(n, LJN                                     , 0x35'36)  /* "56" */ \
    x(n, Matchbox                                , 0x35'37)  /* "57" */ \
    x(n, Mattel                                  , 0x35'38)  /* "58" */ \
    x(n, MiltonBradleyCompany                    , 0x35'39)  /* "59" */ \
    x(n, TitusInteractive                        , 0x36'30)  /* "60" */ \
    x(n, VirginGamesLtd                          , 0x36'31)  /* "61" */ \
    x(n, LucasfilmGames                          , 0x36'34)  /* "64" */ \
    x(n, OceanSoftware                           , 0x36'37)  /* "67" */ \
    x(n, ElectronicArts_2                        , 0x36'39)  /* "69" */ \
    x(n, Infogrames                              , 0x37'30)  /* "70" */ \
    x(n, InterplayEntertainment                  , 0x37'31)  /* "71" */ \
    x(n, Broderbund                              , 0x37'32)  /* "72" */ \
    x(n, SculpturedSoftware                      , 0x37'33)  /* "73" */ \
    x(n, TheSalesCurveLimited                    , 0x37'35)  /* "75" */ \
    x(n, THQ                                     , 0x37'38)  /* "78" */ \
    x(n, Accolade                                , 0x37'39)  /* "79" */ \
    x(n, MisawaEntertainment                     , 0x38'30)  /* "80" */ \
    x(n, lozc                                    , 0x38'33)  /* "83" */ \
    x(n, TokumaShoten                            , 0x38'36)  /* "86" */ \
    x(n, TsukudaOriginal                         , 0x38'37)  /* "87" */ \
    x(n, ChunsoftCo                              , 0x39'31)  /* "91" */ \
    x(n, VideoSystem                             , 0x39'32)  /* "92" */ \
    x(n, OceanSoftwareAcclaimEntertainment_2     , 0x39'33)  /* "93" */ \
    x(n, Varie                                   , 0x39'35)  /* "95" */ \
    x(n, YonezawaSPal                            , 0x39'36)  /* "96" */ \
    x(n, Kaneko                                  , 0x39'37)  /* "97" */ \
    x(n, PackInVideo                             , 0x39'39)  /* "99" */ \
    x(n, BottomUp                                , 0x39'48)  /* "9H" */ \
    x(n, KonamiYuGiOh                            , 0x41'34)  /* "A4" */ \
    x(n, MTO                                     , 0x42'4C)  /* "BL" */ \
    x(n, Kodansha                                , 0x44'4B)  /* "DK" */

CREATE_ENUM_WITH_UTILS(NewLicensingCodeSeq, NewLicensingCode);

#undef NewLicensingCodeSeq
    

/**
 * @brief These are the values used to check what cartridge type is inserted.
 */
#define CartridgeTypeSeq(x, n) \
    x(n, ROM_ONLY                        , 0x00)  /* No MBC, only ROM, no memory mapping, no battery                                          */ \
    x(n, MBC1                            , 0x01)  /* Basic memory bank controller, 2 ROM banks, up to 2 RAM banks                             */ \
    x(n, MBC1_RAM                        , 0x02)  /* MBC1 with RAM support (but no battery)                                                   */ \
    x(n, MBC1_RAM_BATTERY                , 0x03)  /* MBC1 with RAM and battery backup                                                         */ \
    x(n, MBC2                            , 0x05)  /* Simple MBC used in some early GameBoy games, supports 512 bytes of RAM                   */ \
    x(n, MBC2_BATTERY                    , 0x06)  /* MBC2 with battery backup for saved data                                                  */ \
    x(n, ROM_RAM                         , 0x08)  /* ROM and RAM memory without a battery                                                     */ \
    x(n, ROM_RAM_BATTERY                 , 0x09)  /* ROM and RAM with battery backup                                                          */ \
    x(n, MMM01                           , 0x0B)  /* Memory mapper for some third-party games                                                 */ \
    x(n, MMM01_RAM                       , 0x0C)  /* MMM01 with additional RAM                                                                */ \
    x(n, MMM01_RAM_BATTERY               , 0x0D)  /* MMM01 with RAM and battery support                                                       */ \
    x(n, MBC3_TIMER_BATTERY              , 0x0F)  /* MBC3 with a real-time clock (RTC) and battery backup                                     */ \
    x(n, MBC3_TIMER_RAM_BATTERY          , 0x10)  /* MBC3 with RTC, RAM, and battery backup                                                   */ \
    x(n, MBC3                            , 0x11)  /* MBC3 with up to 8 ROM banks and 8 RAM banks, typically used in RPGs                      */ \
    x(n, MBC3_RAM                        , 0x12)  /* MBC3 with RAM support                                                                    */ \
    x(n, MBC3_RAM_BATTERY                , 0x13)  /* MBC3 with RAM and battery backup                                                         */ \
    x(n, MBC5                            , 0x19)  /* MBC5, a more advanced memory bank controller with 32k RAM support, common in later games */ \
    x(n, MBC5_RAM                        , 0x1A)  /* MBC5 with RAM support                                                                    */ \
    x(n, MBC5_RAM_BATTERY                , 0x1B)  /* MBC5 with RAM and battery backup                                                         */ \
    x(n, MBC5_RUMBLE                     , 0x1C)  /* MBC5 with rumble pack support                                                            */ \
    x(n, MBC5_RUMBLE_RAM                 , 0x1D)  /* MBC5 with rumble and RAM support                                                         */ \
    x(n, MBC5_RUMBLE_RAM_BATTERY         , 0x1E)  /* MBC5 with rumble, RAM, and battery backup                                                */ \
    x(n, MBC6                            , 0x20)  /* A memory bank controller that supports more advanced features, less common               */ \
    x(n, MBC7_SENSOR_RUMBLE_RAM_BATTERY  , 0x22)  /* MBC7 with sensor and rumble, RAM, and battery support                                    */ \
    x(n, POCKET_CAMERA                   , 0xFC)  /* A special type for the GameBoy Pocket Camera accessory                                   */ \
    x(n, BANDAI_TAMA5                    , 0xFD)  /* Memory mapping used for Bandai’s TAMA5 system games                                      */ \
    x(n, HUC3                            , 0xFE)  /* A special memory controller used in some games, notably for the Hudson Soft games        */ \
    x(n, HUC1_RAM_BATTERY                , 0xFF)  /* HuC1 controller with RAM and battery backup (used in some RPGs)                          */ \

CREATE_ENUM_WITH_UTILS(CartridgeTypeSeq, CartridgeType);

#undef CartridgeTypeSeq

/**
 * @brief The amount of ROM bytes present on the cartridge.
 * The total size can be found by doing "32 KiB × (1 << <value>)". 
 */
#define RomSizeSeq(x, n) \
                        /* Size,    Number of ROM banks */ \
    x(n, KiB32  , 0x00)	/* 32 KiB	2 (no banking) */ \
    x(n, KiB64  , 0x01)	/* 64 KiB	4   */ \
    x(n, KiB128 , 0x02)	/* 128 KiB	8   */ \
    x(n, KiB256 , 0x03)	/* 256 KiB	16  */ \
    x(n, KiB512 , 0x04)	/* 512 KiB	32  */ \
    x(n, MiB1   , 0x05)	/* 1 MiB	64  */ \
    x(n, MiB2   , 0x06)	/* 2 MiB	128 */ \
    x(n, MiB4   , 0x07)	/* 4 MiB	256 */ \
    x(n, MiB8   , 0x08)	/* 8 MiB	512 */ \

CREATE_ENUM_WITH_UTILS(RomSizeSeq, RomSize);

#undef RomSizeSeq

/**
 * @brief How much RAM is present on the cartridge, if any.
 * If the cartridge type does not include “RAM” in its name, this should be set to 0
 */
#define RamSizeSeq(x, n) \
                            /* Size,    Number of RAM banks    */ \
    x(n, NoRam   , 0x00)	/* 0        No Banks               */ \
    x(n, Unused  , 0x01)	/* -        Unused                 */ \
    x(n, KiB8    , 0x02)	/* 8 KiB    1 bank                 */ \
    x(n, KiB32   , 0x03)	/* 32 KiB	4 banks of 8 KiB each  */ \
    x(n, KiB64   , 0x05)	/* 64 KiB	8 banks of 8 KiB each  */ \
    x(n, KiB128  , 0x04)	/* 128 KiB	16 banks of 8 KiB each */ \

CREATE_ENUM_WITH_UTILS(RamSizeSeq, RamSize);

#undef RamSizeSeq


/**
 * @brief This byte specifies whether this version of the game is intended to be sold in Japan or elsewhere.
 */

#define DestinationCodeSeq(x, n) \
    x(n, Japan       , 0x00) /* This cartridge was meant to be sold in Japan (and possibly overseas) */ \
    x(n, Overseas    , 0x01) /* This cartridge was meant to be sold overseas                         */ \

CREATE_ENUM_WITH_UTILS(DestinationCodeSeq, DestinationCode);

#undef DestinationCodeSeq

/**
 * @brief These are the old licensing codes inside the header of a ROM.
 * They are encoded as a single byte values.
 * 
 * @note There is a special value (0x33) which, if found,
 * means to refer to the new Licensing code to get the correct licensing company.
 * @note Some licensing codes contain the same company name,
 * to prevent collisions and underscore with a number is added.
 */

#define OldLicensingCodeSeq(x, n) \
    x(n, None                    , 0x00) \
    x(n, Nintendo                , 0x01) \
    x(n, Capcom                  , 0x08) \
    x(n, HotB                    , 0x09) \
    x(n, Jaleco                  , 0x0A) \
    x(n, Coconuts                , 0x0B) \
    x(n, EliteSystems            , 0x0C) \
    x(n, ElectronicArts          , 0x13) \
    x(n, Hudsonsoft              , 0x18) \
    x(n, ITCEntertainment        , 0x19) \
    x(n, Yanoman                 , 0x1A) \
    x(n, Clary                   , 0x1D) \
    x(n, Virgin                  , 0x1F) \
    x(n, PCMComplete             , 0x24) \
    x(n, SanX                    , 0x25) \
    x(n, KotobukiSystems         , 0x28) \
    x(n, Seta                    , 0x29) \
    x(n, Infogrames              , 0x30) \
    x(n, Nintendo_2              , 0x31) \
    x(n, Bandai                  , 0x32) \
    x(n, UseNewLicenseeCode      , 0x33) \
    x(n, Konami                  , 0x34) \
    x(n, Hector                  , 0x35) \
    x(n, Capcom_2                , 0x38) \
    x(n, Banpresto               , 0x39) \
    x(n, EntertainmentInteractive, 0x3C) \
    x(n, Gremlin                 , 0x3E) \
    x(n, UbiSoft                 , 0x41) \
    x(n, Atlus                   , 0x42) \
    x(n, Malibu                  , 0x44) \
    x(n, Angel                   , 0x46) \
    x(n, SpectrumHoloby          , 0x47) \
    x(n, Irem                    , 0x49) \
    x(n, Virgin_2                , 0x4A) \
    x(n, Malibu_2                , 0x4D) \
    x(n, USGold                  , 0x4F) \
    x(n, Absolute                , 0x50) \
    x(n, Acclaim                 , 0x51) \
    x(n, Activision              , 0x52) \
    x(n, AmericanSammy           , 0x53) \
    x(n, Gametek                 , 0x54) \
    x(n, ParkPlace               , 0x55) \
    x(n, LJN                     , 0x56) \
    x(n, Matchbox                , 0x57) \
    x(n, MiltonBradley           , 0x59) \
    x(n, Mindscape               , 0x5A) \
    x(n, Romstar                 , 0x5B) \
    x(n, NaxatSoft               , 0x5C) \
    x(n, Tradewest               , 0x5D) \
    x(n, Titus                   , 0x60) \
    x(n, Virgin_3                , 0x61) \
    x(n, Ocean                   , 0x67) \
    x(n, ElectronicArts_2        , 0x69) \
    x(n, EliteSystems_2          , 0x6E) \
    x(n, ElectroBrain            , 0x6F) \
    x(n, Infogrames_2            , 0x70) \
    x(n, Interplay               , 0x71) \
    x(n, Broderbund              , 0x72) \
    x(n, SculpturedSoft          , 0x73) \
    x(n, TheSalesCurve           , 0x75) \
    x(n, THQ                     , 0x78) \
    x(n, Accolade                , 0x79) \
    x(n, TriffixEntertainment    , 0x7A) \
    x(n, Microprose              , 0x7C) \
    x(n, Kemco                   , 0x7F) \
    x(n, MisawaEntertainment     , 0x80) \
    x(n, LOZC                    , 0x83) \
    x(n, TokumaShoten            , 0x86) \
    x(n, BulletProofSoftware     , 0x8B) \
    x(n, VicTokai                , 0x8C) \
    x(n, Ape                     , 0x8E) \
    x(n, Imax                    , 0x8F) \
    x(n, ChunSoft                , 0x91) \
    x(n, VideoSystem             , 0x92) \
    x(n, Tsuburava               , 0x93) \
    x(n, Varie                   , 0x95) \
    x(n, Yonezawa                , 0x96) \
    x(n, Kaneko                  , 0x97) \
    x(n, Arc                     , 0x99) \
    x(n, NihonBussan             , 0x9A) \
    x(n, Tecmo                   , 0x9B) \
    x(n, Imagineer               , 0x9C) \
    x(n, Banpresto_2             , 0x9D) \
    x(n, Nova                    , 0x9F) \
    x(n, HoriElectric            , 0xA1) \
    x(n, Bandai_2                , 0xA2) \
    x(n, Konami_2                , 0xA4) \
    x(n, Kawada                  , 0xA6) \
    x(n, Takara                  , 0xA7) \
    x(n, TechnosJapan            , 0xA9) \
    x(n, Broderbund_2            , 0xAA) \
    x(n, ToeiAnimation           , 0xAC) \
    x(n, Toho                    , 0xAD) \
    x(n, Namco                   , 0xAF) \
    x(n, Acclaim_2               , 0xB0) \
    x(n, ASCIIOrNexoft           , 0xB1) \
    x(n, Bandai_3                , 0xB2) \
    x(n, Enix                    , 0xB4) \
    x(n, Hal                     , 0xB6) \
    x(n, SNK                     , 0xB7) \
    x(n, PonyCanyon              , 0xB9) \
    x(n, CultureBrain            , 0xBA) \
    x(n, Sunsoft                 , 0xBB) \
    x(n, SonyImagesoft           , 0xBD) \
    x(n, Sammy                   , 0xBF) \
    x(n, Taito_2                 , 0xC0) \
    x(n, Kemco_2                 , 0xC2) \
    x(n, Squaresoft              , 0xC3) \
    x(n, TokumaShoten_2          , 0xC4) \
    x(n, DataEast                , 0xC5) \
    x(n, TonkinHouse             , 0xC6) \
    x(n, Koei                    , 0xC8) \
    x(n, UFL                     , 0xC9) \
    x(n, Ultra                   , 0xCA) \
    x(n, Vap                     , 0xCB) \
    x(n, USE                     , 0xCC) \
    x(n, Meldac                  , 0xCD) \
    x(n, PonyCanyon_2            , 0xCE) \
    x(n, Angel_2                 , 0xCF) \
    x(n, Taito_3                 , 0xD0) \
    x(n, Sofel                   , 0xD1) \
    x(n, Quest                   , 0xD2) \
    x(n, SigmaEnterprises        , 0xD3) \
    x(n, AskKodansha             , 0xD4) \
    x(n, NaxatSoft_2             , 0xD6) \
    x(n, CopyaSystems            , 0xD7) \
    x(n, Banpresto_3             , 0xD9) \
    x(n, Tomy                    , 0xDA) \
    x(n, LJN_2                   , 0xDB) \
    x(n, NCS                     , 0xDD) \
    x(n, Human                   , 0xDE) \
    x(n, Altron                  , 0xDF) \
    x(n, Jaleco_2                , 0xE0) \
    x(n, Towachiki               , 0xE1) \
    x(n, Uutaka                  , 0xE2) \
    x(n, Varie_2                 , 0xE3) \
    x(n, Epoch                   , 0xE5) \
    x(n, Athena                  , 0xE7) \
    x(n, Asmik                   , 0xE8) \
    x(n, Natsume                 , 0xE9) \
    x(n, KingRecords             , 0xEA) \
    x(n, Atlus_2                 , 0xEB) \
    x(n, EpicRecords             , 0xEC) \
    x(n, IGS                     , 0xEE) \
    x(n, AWave                   , 0xF0) \
    x(n, ExtremeEntertainment    , 0xF3) \
    x(n, LJN_3                   , 0xFF) \

CREATE_ENUM_WITH_UTILS(OldLicensingCodeSeq, OldLicensingCode);

#undef OldLicensingCodeSeq

