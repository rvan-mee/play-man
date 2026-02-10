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

#include <play-man/gameboy/cartridge/Cartridge.hpp>
#include <play-man/gameboy/memory/MemoryBus.hpp>
#include <play-man/gameboy/opcodes/Opcodes.hpp>
#include <play-man/gameboy/cpu/Instruction.hpp>
#include <play-man/containers/EnumIndexableArray.hpp>

#include <functional>
#include <stdint.h>

namespace GameBoy
{	
    class Cpu
    {
        friend struct TestFixtures::GameBoyCpuFixture;

        using InstructionPrototype = Instruction::InstructionPrototype; /*!< -. */
    
    private:
        std::shared_ptr<ACartridge>     cartridge;
        CpuCore                         core;
        MemoryBus                       memoryBus;

        size_t cycles = 0;
        Instruction currentInstruction; /*< The current instruction to execute/is being executed. */

        static constexpr size_t numberOfInstructions = 256;
        static constexpr size_t numberOfPrefixedInstructions = 256;

        EnumIndexableArray<OpCode, InstructionPrototype, numberOfInstructions> instructions;
        EnumIndexableArray<PrefixedOpCode, InstructionPrototype, numberOfPrefixedInstructions> prefixedInstructions;

    public:

        Cpu() = delete;
        Cpu(std::shared_ptr<ACartridge> _cartridge) : cartridge(_cartridge), memoryBus(cartridge, core)
        {
            InitInstructionTable();
        };

        /**
         * @brief Used for testing, overwrites the current ROM data with the data
         *        found inside the file at filePath. Also resets all the internal registers.
         * @param filePath The location of the test rom.
         */
        void LoadTestRom(const char *filePath);

        /**
         * @brief -.
         * @param opCode 
         */
        void ExecuteInstruction(OpCode opCode);

        /**
         * @brief -.
         * @param opCode 
         */
        void ExecuteInstruction(PrefixedOpCode opCode);

        /**
         * @brief Logs the current instruction being executed in debug mode.
         */
        void LogInstruction();

        /**
         * @brief Executes the current instruction.
         */
        void ExecuteInstruction();

        /**
         * @brief Fetches the instruction located on the address stored inside the PC register.
         */
        void FetchInstruction();

        /*!
         * @brief Reads byte from memory bus at address.
         * @brief address
         * @return
        */
        uint8_t Fetch(uint16_t address);

        /**
         * @brief Reads byte from memory bus at address contained in PC; increments PC;
         * @return 
         */
        uint8_t FetchPcAddress();

        /**
         * @brief Reads two bytes from the memory bus at address contained in PC; increments PC by 2;
         * 
         * @note The first byte will be the upper byte of the final 16 bit value.
         * 
         * @return The 16bit data found at the address contained in PC.
         */
        uint16_t FetchPcAddress16bit();

        /**
         * @brief Initializes the instruction array.
         */
        void InitInstructionTable();

//////////////////
// Instructions //
//////////////////
private:

        /**
         * @brief Invalid OpCodes result in the CPU hard-locking itself until powered off
         */
        size_t HardLock();

        /**
		 * @brief
		 * @return number of cycles.
		 */
        size_t NOP();

        /**
         * @brief Adjusts the A register to a binary-coded decimal number.
         * 
         * @note binary-coded decimal number utilizes 4 bits for the numbers 0 through 9.
         *       This allows the 8 bit register to go from 0 to 99, using the lower half
         *       of the register for the x9 numbers and the top half 9x numbers.
         * 
         * @note https://blog.ollien.com/posts/gb-daa/
         * 
         * @return number of cycles.
         */
        size_t DDA();

        /**
         * @brief Applies the one's complement on register A.
         *        (flips all the bits).
         * 
         * @return number of cycles.
         */
        size_t CPL();

        /**
         * @brief Sets the carry flag inside the F register.
         * 
         * @note Also sets the SUB and HALF_CARRY flags to false.
         * 
         * @return number of cycles.
         */
        size_t SCF();

        /**
         * @brief Flips the carry flag.
         * 
         * @note Also sets the SUB and HALF_CARRY flags to false.
         * 
         * @return number of cycles.
         */
        size_t CCF();

        /**
         * @brief Returns to the previous subroutine by popping the previous saved
         * ProgramCounter from the stack.
         * 
         * @return number of cycles.
         */
        size_t Return();

        /**
         * @brief Returns to the previous subroutine by popping the previous saved
         * ProgramCounter from the stack.
         * 
         * @return number of cycles.
         */
        size_t ConditionalReturn(FlagRegisterFlag conditionalFlag, bool flagEnabled);

        /**                               Jmp/call instructions                                             **/

        /**
         * @brief Loads the address value addrReg into the PC reg.
         * 
         * @param addrReg Pointer to the register containing the new PC value.
         * 
         * @return number of cycles.
         */
        size_t Jump_Addr(RegisterPointer addrReg);

        /**
         * @brief Loads two bytes of immediate date into the PC reg.
         *        First byte of immediate data is low byte.
         * @return number of cycles.
         */
        size_t Jump_16bit_ImmediateData();

        /**
         * @brief Increments the PC register by the 8 bit value of the immediate data.
         * @return number of cycles.
         */
        size_t Jump_Relative_8bit_SignedImmediateData();

        /**
         * @brief Check if the flag is set according to the given condition, increments the PC register
         *        by the signed 8 bit value of the immediate data.
         * 
         * @param flag The flag to check.
         * @param flagCondition The condition the flag needs to be in for the jump to happen.
         * 
         * @return number of cycles.
         */
	    size_t Jump_Relative_Conditional_8bit_SignedImmediateData(FlagRegisterFlag flag, bool flagCondition);

        /**
         * @brief Check if the flag is set according to the given condition, sets the PC register
         *        to the 16 bit value of the immediate data.
         * 
         * @param flag The flag to check.
         * @param flagCondition The condition the flag needs to be in for the jump to happen.
         * 
         * @return number of cycles.
         */
        size_t Jump_Conditional_16bit_ImmediateData(FlagRegisterFlag flag, bool flagCondition);

        /**
         * @brief Pushes the PC register to the stack, after which the 16bit immediate data will
         * be loaded into PC.
         * 
         * @return number of cycles.
         */
        size_t Call_16bit_ImmediateData();

        /**
         * @brief If the flag condition is met the PC register value will be pushed to the stack,
         * after which the 16bit immediate data will be loaded into PC.
         * 
         * @param flag The flag to check.
         * @param flagCondition The condition the flag needs to be in for the jump to happen.
         * 
         * @return number of cycles.
         */
        size_t ConditionalCall_16bit_ImmediateData(FlagRegisterFlag flag, bool flagCondition);

        /**                                16 bit instructions                                             **/

        /**
         * @brief Loads the contents 16 bits of fromReg into toReg.
         * 
         * @param destReg  Pointer to the register where the data is loaded into.
         * @param fromReg  Pointer to the register where the data is taken from.
         * 
         * @return number of cycles.
         */
        size_t Load_16bit(RegisterPointer destReg, RegisterPointer fromReg);

		/**
		 * @brief Loads two bytes of immediate data into reg.
		 * 		  First byte of immediate data is low byte.
		 * @param reg 
		 * @return size_t 
		 */
		size_t Load_16bit_ImmediateData(RegisterPointer reg);

        /**
         * @brief Increments the 16 bit register by 1.
         * 
         * @param reg Pointer to the register needing to be incremented.
         * @return number of cycles.
         */
        size_t Increment_16bit(RegisterPointer reg);

        /**
         * @brief Decrements the 16 bit register by 1.
         * 
         * @param reg Pointer to the register needing to be decremented.
         * @return number of cycles.
         */
        size_t Decrement_16bit(RegisterPointer reg);

        /**
         * @brief Loads the contents of the 16bit register to the address found at PC.
         * 
         * @param reg Pointer to the register where the data is taken from.
         * 
         * @return number of cycles.
         */
        size_t Load_16bit_RegToImmediateAddr(RegisterPointer reg);

        /**
         * @brief Adds the contents from fromReg to the toReg.
         * 
         * @note Sets the S flag to false.
         * @note Sets the H flag according to the calculation.
         * @note Sets the C flag according to the calculation.
         * 
         * @return number of cycles.
         */
        size_t Add_16bit(RegisterPointer toReg, RegisterPointer fromReg);

        /**
         * @brief Adds the 8 bit signed value found at the program counter (PC) to the
         *        16 bit register reg.
         * 
         * @note Sets the Z flag to false.
         * @note Sets the S flag to false.
         * @note Sets the H flag according to the calculation (Lower register carry).
         * @note Sets the C flag according to the calculation (Lower register carry).
         * 
         * @return number of cycles.
         */
        size_t Add_16bit_8bitSignedImmediateData(RegisterPointer reg);

        /**
         * @brief Pushes the 16 bit value found inside reg to the stack, after which the
         *        StackPointer (SP) will be decremented by 2.
         * 
         * @return number of cycles.
         */
        size_t Push(RegisterPointer reg);

        /**
         * @brief Pops the 16 bit value found at the address stored inside StackPointer (SP)
         *        inside reg, after which the StackPointer will be incremented by 2.
         * 
         * @return number of cycles.
         */
        size_t Pop(RegisterPointer reg);

        /**
         * @brief The 'Restart' instruction. Pushes the current program counter (PC) to the stack
         *        and performs a jump to the given address (0x00 as the high part, the address as low part).
         * 
         * @param address The address that will be jumped to.
         * 
         * @return number of cycles.  
         */
        size_t RST(const uint8_t address);

        /**                                8 bit instructions                                              **/

        /**
         * @brief Adds the contents from fromReg to the accumulator register.
         * 
         * @param opReg Pointer to the register that will be used as the operand.
         * @param GetValue Pointer to the operand register's member function that will take the
         *                 High or Low part of the register. 
         * 
         * @note Sets the Z flag according to the calculation.
         * @note Sets the S flag to false.
         * @note Sets the H flag according to the calculation.
         * @note Sets the C flag according to the calculation.
         * 
         * @return number of cycles.
         */
        size_t Add_8bit(RegisterPointer opReg, RegisterGet8Bit GetValue);

        /**
         * @brief Adds the contents from memory location addrReg to the accumulator register.
         * 
         * @param addrReg Pointer to the register which contains the location of 
         *                the memory that will be used for the addition.
         * 
         * @note Sets the Z flag according to the calculation.
         * @note Sets the S flag to false.
         * @note Sets the H flag according to the calculation.
         * @note Sets the C flag according to the calculation.
         * 
         * @return number of cycles.
         */
        size_t Add_8bit_Addr(RegisterPointer addrReg);

        /**
         * @brief Adds the 8 bit immediate data found at the memory location stored
         *        inside the program counter (PC) to the accumulator register.
         * 
         * @note Sets the Z flag according to the calculation.
         * @note Sets the S flag to false.
         * @note Sets the H flag according to the calculation.
         * @note Sets the C flag according to the calculation.
         * 
         * @return number of cycles.
         */
        size_t Add_8bit_ImmediateData();

        /**
         * @brief Adds the contents from opReg to the accumulator register, combined with the 
         *        contents of the Carry flag (1 or 0).
         * 
         * @param opReg Pointer to the register that will be used as the operand.
         * @param GetValue Pointer to the operand register's member function that will take the
         *                 High or Low part of the register.
         * 
         * @note Sets the Z flag according to the calculation.
         * @note Sets the S flag to false.
         * @note Sets the H flag according to the calculation.
         * @note Sets the C flag according to the calculation.
         * 
         * @return number of cycles.
         */
        size_t AddCarry_8bit(RegisterPointer opReg, RegisterGet8Bit GetValue);

        /**
         * @brief Adds the value found at the current program counter (PC) to the accumulator register,
         *        combined with the contents of the Carry flag (1 or 0).
         * 
         * @note Sets the Z flag according to the calculation.
         * @note Sets the S flag to false.
         * @note Sets the H flag according to the calculation.
         * @note Sets the C flag according to the calculation.
         * 
         * @return number of cycles.
         */
        size_t AddCarry_8bit_ImmediateData();

        /**
         * @brief Adds the contents from memory location addrReg to the accumulator register, combined with the 
         *        contents of the Carry flag (1 or 0).
         * 
         * @param addrReg Pointer to the register which contains the location of 
         *                the memory that will be used for the addition.
         * 
         * @note Sets the Z flag according to the calculation.
         * @note Sets the S flag to false.
         * @note Sets the H flag according to the calculation.
         * @note Sets the C flag according to the calculation.
         * 
         * @return number of cycles.
         */
        size_t AddCarry_8bit_Addr(RegisterPointer addrReg);

        /**
         * @brief Subtracts the contents of opReg from the accumulator register.
         * 
         * @param opReg Pointer to the register that will be used as the operand.
         * @param GetValue Pointer to the operand register's member function that will take the
         *                 High or Low part of the register.
         * 
         * @note Sets the Z flag according to the calculation.
         * @note Sets the S flag to true.
         * @note Sets the H flag according to the calculation.
         * @note Sets the C flag according to the calculation.
         * 
         * @return number of cycles.
         */
        size_t Sub_8bit(RegisterPointer opReg, RegisterGet8Bit GetValue);

        /**
         * @brief Subtracts the contents found on the address contained inside the program counter (PC)
         *        from the accumulator register.
         * 
         * @note Sets the Z flag according to the calculation.
         * @note Sets the S flag to true.
         * @note Sets the H flag according to the calculation.
         * @note Sets the C flag according to the calculation.
         * 
         * @return number of cycles.
         */
        size_t Sub_8bit_ImmediateData();

        /**
         * @brief Subtracts the contents of memory location addrReg
         *        from the accumulator register.
         * 
         * @param addrReg Pointer to the register which contains the location of 
         *                the memory that will be used for the subtraction.
         * 
         * @note Sets the Z flag according to the calculation.
         * @note Sets the S flag to true.
         * @note Sets the H flag according to the calculation.
         * @note Sets the C flag according to the calculation.
         * 
         * @return number of cycles.
         */
        size_t Sub_8bit_Addr(RegisterPointer addrReg);

        /**
         * @brief Subtracts the contents of fromReg combined with the contents of
         *        the Carry flag (1 or 0) from the accumulator register.
         * 
         * @param opReg Pointer to the register that will be used as the operand.
         * @param GetValue Pointer to the operand register's member function that will take the
         *                 High or Low part of the register.
         * 
         * @note Sets the Z flag according to the calculation.
         * @note Sets the S flag to true.
         * @note Sets the H flag according to the calculation.
         * @note Sets the C flag according to the calculation.
         * 
         * @return number of cycles.
         */
        size_t SubCarry_8bit(RegisterPointer opReg, RegisterGet8Bit GetValue);

        /**
         * @brief Subtracts the contents found at the addres inside the program counter (PC)
         *        combined with the contents of the Carry flag (1 or 0) from the accumulator register.
         * 
         * @note Sets the Z flag according to the calculation.
         * @note Sets the S flag to true.
         * @note Sets the H flag according to the calculation.
         * @note Sets the C flag according to the calculation.
         * 
         * @return number of cycles.
         */
        size_t SubCarry_8bit_ImmediateData();

        /**
         * @brief Subtracts the contents of memory location addrReg combined
         *        with the contents of the Carry flag (1 or 0) from the accumulator register.
         * 
         * @param addrReg Pointer to the register which contains the location of 
         *                the memory that will be used for the subtraction.
         * 
         * @note Sets the Z flag according to the calculation.
         * @note Sets the S flag to true.
         * @note Sets the H flag according to the calculation.
         * @note Sets the C flag according to the calculation.
         * 
         * @return number of cycles.
         */
        size_t SubCarry_8bit_Addr(RegisterPointer addrReg);

        /**
         * @brief Increments the data inside the provided register by 1.
         * 
         * @param opReg Pointer to the register that will be used as the operand.
         * @param GetValue Pointer to the operand register's member function that will take the
         *                 High or Low part of the register.
         * @param SetValue Pointer to the register where the resulting value will be set.
         * 
         * @note The Z flag is set if the incrementation results in a 0.
         * @note The S flag is set to false.
         * @note The H flag is set according to the calculation.
         * 
         * @return number of cycles. 
         */
        size_t Increment_8bit(RegisterPointer opReg, RegisterGet8Bit GetValue, RegisterSet8Bit SetValue);

        /**
         * @brief Increments the value pointed to by addrReg by one.
         * 
         * @param addrReg Pointer the the register containing the address of the value
         *                that needs to be incremented.
         * 
         * @note The Z flag is set if the increment results in a 0.
         * @note the S flag is set to false.
         * @note the H flag is set according to the calculation.
         */
        size_t Increment_Dereferenced(RegisterPointer addrReg);

        /**
         * @brief Decrements the data inside the provided register by 1.
         * 
         * @param opReg Pointer to the register that will be used as the operand.
         * @param GetValue Pointer to the operand register's member function that will take the
         *                 High or Low part of the register.
         * @param SetValue Pointer to the register where the resulting value will be set.
         * 
         * @note The Z flag is set if the decrementation results in a 0.
         * @note The S flag is set to true.
         * @note The H flag is set according to the calculation.
         * 
         * @return number of cycles. 
         */
        size_t Decrement_8bit(RegisterPointer opReg, RegisterGet8Bit GetValue, RegisterSet8Bit SetValue);

        /**
         * @brief Decrements the value pointed to by addrReg by one.
         * 
         * @param addrReg Pointer to the register containing the address of the value
         *                that needs to be decremented.
         * 
         * @note The Z flag is set if the decrement results in a 0.
         * @note The S flag is set to true.
         * @note The H flag is set according to the calculation.
         */
        size_t Decrement_Dereferenced(RegisterPointer addrReg);

        /**
         * @brief Performs a bitwise AND on the accumulator register with the
         *        data inside oppReg as the operant.
         * 
         * @param oppReg Pointer to the register containing the operant that will be
         *               used for the bitwise operation.
         * @param GetValue Pointer to the operand register's member function that will take the
         *                 High or Low part of the register.
         * 
         * @note The Z flag is set if the AND results in a 0.
         * @note The S flag is set to false.
         * @note The H flag is set to true.
         * @note The C flag is set to false.
         */
        size_t BitwiseAnd(RegisterPointer reg, RegisterGet8Bit GetOperand);

        /**
         * @brief Performs a bitwise AND on the accumulator register with the
         *        memory found at the location inside of addrReg as the operant.
         * 
         * @param adrReg Pointer to the register containing the memory address of the operant
         *               that will be used for the bitwise operation.
         * 
         * @note The Z flag is set if the AND results in a 0.
         * @note The S flag is set to false.
         * @note The H flag is set to true.
         * @note The C flag is set to false.
         */
        size_t BitwiseAnd_Addr(RegisterPointer addrReg);

        /**
         * @brief Performs a bitwise AND on the accumulator register with the
         *        data found at the current program counter (PC).
         * 
         * @note The Z flag is set if the AND results in a 0.
         * @note The S flag is set to false.
         * @note The H flag is set to true.
         * @note The C flag is set to false.
         */
        size_t BitwiseAnd_ImmediateData();

        /**
         * @brief Performs a bitwise XOR on the accumulator register with the
         *        data inside oppReg as the operant.
         * 
         * @param oppReg Pointer to the register containing the operant that will be
         *               used for the bitwise operation.
         * @param GetValue Pointer to the operand register's member function that will take the
         *                 High or Low part of the register.
         * 
         * @note The Z flag is set if the XOR results in a 0.
         * @note The S flag is set to false.
         * @note The H flag is set to true.
         * @note The C flag is set to false.
         */
        size_t BitwiseXor(RegisterPointer reg, RegisterGet8Bit GetOperand);

        /**
         * @brief Performs a bitwise XOR on the accumulator register with the
         *        memory found at the location inside of addrReg as the operant.
         * 
         * @param adrReg Pointer to the register containing the memory address of the operant
         *               that will be used for the bitwise operation.
         * 
         * @note The Z flag is set if the XOR results in a 0.
         * @note The S flag is set to false.
         * @note The H flag is set to true.
         * @note The C flag is set to false.
         */
        size_t BitwiseXor_Addr(RegisterPointer addrReg);

        /**
         * @brief Performs a bitwise XOR on the accumulator register with the
         *        data found at the current program counter (PC).
         * 
         * @note The Z flag is set if the XOR results in a 0.
         * @note The S flag is set to false.
         * @note The H flag is set to true.
         * @note The C flag is set to false.
         */
        size_t BitwiseXor_ImmediateData();

        /**
         * @brief Performs a bitwise OR on the accumulator register with the
         *        data inside oppReg as the operant.
         * 
         * @param oppReg Pointer to the register containing the operant that will be
         *               used for the bitwise operation.
         * @param GetValue Pointer to the operand register's member function that will take the
         *                 High or Low part of the register.
         * 
         * @note The Z flag is set if the OR results in a 0.
         * @note The S flag is set to false.
         * @note The H flag is set to true.
         * @note The C flag is set to false.
         */
        size_t BitwiseOr(RegisterPointer reg, RegisterGet8Bit GetOperand);

        /**
         * @brief Performs a bitwise OR on the accumulator register with the
         *        memory found at the location inside of addrReg as the operant.
         * 
         * @param adrReg Pointer to the register containing the memory address of the operant
         *               that will be used for the bitwise operation.
         * 
         * @note The Z flag is set if the OR results in a 0.
         * @note The S flag is set to false.
         * @note The H flag is set to false.
         * @note The C flag is set to false.
         */
        size_t BitwiseOr_Addr(RegisterPointer addrReg);

        /**
         * @brief Performs a bitwise OR on the accumulator register with the
         *        data found at the current program counter (PC).
         * 
         * @note The Z flag is set if the OR results in a 0.
         * @note The S flag is set to false.
         * @note The H flag is set to false.
         * @note The C flag is set to false.
         */
        size_t BitwiseOr_ImmediateData();

        /**
         * @brief Stores the byte found in the register of dataReg to the memory location pointed to by the
         *        immediate address found inside the location pointed to by the program counter (PC).
         * 
         * @note The 8 bits of immediate data are added to the value of 0xFF'00 to get the final address.
         * 
         * @param dataReg Pointer to the register where the data will be taken from.
         * @param GetValue Pointer to the dataReg register's member function that will take the
         *                 High or Low part of the register.
         * 
         * @return number of cycles.
         */
        size_t Store_8bit_8bitImmediateAddr(RegisterPointer dataReg, RegisterGet8Bit GetData);

        /**
         * @brief Stores the byte found in the register of dataReg to the memory location pointed to by the
         *        immediate address found inside the location pointed to by the program counter (PC).
         *  
         * @param dataReg Pointer to the register where the data will be taken from.
         * @param GetValue Pointer to the dataReg register's member function that will take the
         *                 High or Low part of the register.
         * 
         * @return number of cycles.
         */
        size_t Store_8bit_16bitImmediateAddr(RegisterPointer dataReg, RegisterGet8Bit GetData);

        /**
         * @brief Stores the byte found in the register of dataReg to the memory location
         *        pointed to by the addrReg.
         * 
         * @note The 8 bits address found inside addrReg is added to the value of 0xFF'00 to get the final address.
         * 
         * @param addrReg Pointer to the register containing the address where the data needs to be stored. 
         * @param dataReg Pointer to the register where the data will be taken from.
         * @param GetValue Pointer to the dataReg register's member function that will take the
         *                 High or Low part of the register.
         * 
         * @return number of cycles.
         */
        size_t Store_8bit_8bitAddr(RegisterPointer addrReg, RegisterGet8Bit GetAddr, RegisterPointer dataReg, RegisterGet8Bit GetData);

        /**
         * @brief Stores the byte found in the register of dataReg to the addres Pointed to by addrReg.
         * 
         * @param addrReg Pointer to the register containing the address where the data needs to be stored. 
         * @param dataReg Pointer to the register where the data will be taken from.
         * @param GetValue Pointer to the dataReg register's member function that will take the
         *                 High or Low part of the register.
         * 
         * @return number of cycles.
         */
        size_t Store_8bit_Addr(RegisterPointer addrReg, RegisterPointer dataReg, RegisterGet8Bit GetData);

        /**
         * @brief Stores the byte found in the high register of dataReg to the addres
         *        pointed to by addrReg after which the contents of addrReg gets incremented by 1.
         * 
         * @param addrReg Pointer to the register containing the address where the data needs to be stored. 
         * @param dataReg Pointer to the register where the data will be taken from.
         * @param GetValue Pointer to the dataReg register's member function that will take the
         *                 High or Low part of the register.
         * 
         * @return number of cycles.
         */
        size_t Store_8bit_AddrIncrement(RegisterPointer addrReg, RegisterPointer dataReg, RegisterGet8Bit GetData);

        /**
         * @brief Stores the byte found in the high register of dataReg to the addres
         *        pointed to by addrReg after which the contents of addrReg gets decremented by 1.
         * 
         * @param addrReg Pointer to the register containing the address where the data needs to be stored. 
         * @param dataReg Pointer to the register where the data will be taken from.
         * @param GetValue Pointer to the dataReg register's member function that will take the
         *                 High or Low part of the register.
         * 
         * @return number of cycles.
         */
        size_t Store_8bit_AddrDecrement(RegisterPointer addrReg, RegisterPointer dataReg, RegisterGet8Bit GetData);

        /**
         * @brief Stores the byte found at the current PC register location to the destination
         *        pointed to by addrReg.
         * 
         * @param addrReg pointer to the register containing the address where the data needs to be stored.
         * 
         * @return number of cycles.
         */
        size_t Store_8bit_Addr_ImmediateData(RegisterPointer addrReg);


        /**
         * @brief Adds the signed 8 bits of immediate data to the StackPointer and stores the result inside reg.
         * 
         * @param reg Pointer to the register where the data will to be stored.
         * 
         * @note The Z flag is set to false.
         * @note The S flag is set to false.
         * @note The H flag is set according to the calculation.
         * @note The C flag is set according to the calculation.
         * 
         * @return number of cycles.
         */
        size_t Store_StackPointerPlusSignedImmediateData(RegisterPointer reg);

        /**
         * @brief Loads the 8bit immediate data into the provided register.
         * 
         * @param reg Pointer to the register where the data will be loaded into.
         * @param SetValue Pointer to the register's member function that will store the result inside
         *                 High or Low part of the register.
         * 
         * @return number of cycles. 
         */
        size_t Load_8bit_ImmediateData(RegisterPointer reg, RegisterSet8Bit SetValue);

        /**
         * @brief Loads the 8 bits of memory found at the location pointed to
         *        by the immediate data into the provided register.
         * 
         * @return number of cycles. 
         */
        size_t Load_8bit_ImmediateAddr(RegisterPointer reg, RegisterSet8Bit SetValue);

        /**
         * @brief Loads the 8 bits of memory found at the 8bit immediate data into the provided register.
         * 
         * @note The 8 bits of immediate data is added to the value 0xFF00, resulting in an address
         *       rang of 0xFF00-0xFFFF.
         * 
         * @return number of cycles. 
         */
        size_t Load_8bit_8bitImmediateAddr(RegisterPointer reg, RegisterSet8Bit SetValue);

        /**
         * @brief Loads the 8bits of data found at the addrReg into destReg.
         * 
         * @param destReg  Pointer to the register where the data is loaded into.
         * @param SetValue Member function pointer of destReg, specifying the high or low part of the register. 
         * @param addrReg  Pointer to the register containing the address where the data is located.
         * 
         * @return number of cycles. 
         */
        size_t Load_8bit_Addr(RegisterPointer destReg, RegisterSet8Bit SetValue, RegisterPointer addrReg);

        /**
         * @brief Loads the 8bits of data from the 8bit address found inside addrReg and loads it into destReg.
         * 
         * @note The bit bit address will be added to the value 0xFF00, making an effective address of 0xFF00-0xFFFF.
         * 
         * @param destReg  Pointer to the register where the data is loaded into.
         * @param SetValue Member function pointer of destReg, specifying the high or low part of the register. 
         * @param addrReg  Pointer to the register containing the 8bit address where the data is located.
         * @param GetAddr  Member function pointer of addrReg, specifying the high or low part of the register.
         * 
         * @return number of cycles. 
         */
        size_t Load_8bit_8bitAddr(RegisterPointer destReg, RegisterSet8Bit SetValue, RegisterPointer addrReg, RegisterGet8Bit GetAddr);

        /**
         * @brief Loads the 8 bits of data found at addrReg into destReg, after which the pointer inside
         *        addrReg gets incremented by 1.
         * 
         * @param destReg  Pointer to the register where the data is loaded into.
         * @param SetValue Member function pointer of destReg, specifying the high or low part of the register. 
         * @param addrReg  Pointer to the register containing the address where the data is located.
         *                 Will get incremented by 1 after the data is retrieved.
         * 
         * @return number of cycles.
         */
        size_t Load_8bit_AddrIncrement(RegisterPointer destReg, RegisterSet8Bit SetValue, RegisterPointer addrReg);

        /**
         * @brief Loads the 8 bits of data found at addrReg into destReg, after which the pointer inside
         *        addrReg gets decremented by 1.
         * 
         * @param destReg  Pointer to the register where the data is loaded into.
         * @param SetValue Member function pointer of destReg, specifying the high or low part of the register. 
         * @param addrReg  Pointer to the register containing the address where the data is located.
         *                 Will get decremented by 1 after the data is retrieved.
         * 
         * @return number of cycles.
         */
        size_t Load_8bit_AddrDecrement(RegisterPointer destReg, RegisterSet8Bit SetValue, RegisterPointer addrReg);

        /**
         * @brief Loads the contents 8 bit of fromReg into toReg.
         * 
         * @param destReg  Pointer to the register where the data is loaded into.
         * @param SetValue Member function pointer of destReg, specifying the high or low part of the register. 
         * @param fromReg  Pointer to the register where the data is taken from.
         * @param GetValue Member function pointer of fromReg, specifying the high or low part of the register.
         * 
         * @return number of cycles.
         */
        size_t Load_8bit(RegisterPointer destReg, RegisterSet8Bit SetValue, RegisterPointer fromReg, RegisterGet8Bit GetValue);

        /**
         * @brief Compares the 8 bit value of opReg to the accumulator register
         *        by calculating (A - operand).
         * 
         * @param opReg    Pointer to the register that will be used as the operand.
         * @param GetValue Pointer to the operand register's member function that will take the
         *                 High or Low part of the register.
         * 
         * @note The Z flag is set if they are equal.
         * @note The S flag is set to true.
         * @note The H flag is set according to the calculation.
         * @note The C flag is set according to the calculation.
         * 
         * @return number of cycles.
         */
        size_t Compare_8bit(RegisterPointer opReg, RegisterGet8Bit GetValue);

        /**
         * @brief Compares the 8 bit value found at addrReg to the accumulator register
         *        by calculating (A - operand).
         * 
         * @param addrReg Pointer to the register containing the address of the operand used
         *                within the comparison.
         * 
         * @note The Z flag is set if they are equal.
         * @note The S flag is set to true.
         * @note The H flag is set according to the calculation.
         * @note The C flag is set according to the calculation.
         * 
         * @return number of cycles.
         */
        size_t Compare_8bit_Addr(RegisterPointer addrReg);

        /**
         * @brief Compares the 8 bit immediate data to the accumulator register by calculating (A - operand).
         * 
         * @note The Z flag is set if they are equal.
         * @note The S flag is set to true.
         * @note The H flag is set according to the calculation.
         * @note The C flag is set according to the calculation.
         * 
         * @return number of cycles.
         */
        size_t Compare_8bit_ImmediateData();

        /**
         * @note Non prefixed version of the rotate instruction, difference being the 
         *       cycle count and zero flag not being set compared to the prefixed one.
         * 
         * @brief Rotates the given register's to the left by 1 through the carry flag, that is,
         *        appending the state of the carry flag bit to the right of the same register.
         *        The shifted-out bit will become the new state of the carry flag.
         * 
         * @param reg      Pointer to the register that will be used as the operand.
         * @param GetValue Pointer to the register's member function that will take the
         *                 High or Low part of the register.
         * @param SetValue Pointer to the register's member function that will store the result
         *                 into the High or Low part of the register.
         * 
         * @note The Z flag is set to false.
         * @note The S flag is set to false.
         * @note The H flag is set to false.
         * @note The C flag is set to the shifted-out bit.
         * 
         * @return number of cycles.
         */
        size_t NoPrefixRotateLeft(RegisterPointer reg, RegisterGet8Bit GetValue, RegisterSet8Bit SetValue);

        /**
         * @note Non prefixed version of the rotate instruction, difference being the 
         *       cycle count and zero flag not being set compared to the prefixed one.
         * 
         * @brief Rotates the given register to the left by 1,
         *        appending the shifted-out bit to the right of that same register
         *        and storing that same bit into the carry register.
         * 
         * @param reg      Pointer to the register that will be used as the operand.
         * @param GetValue Pointer to the register's member function that will take the
         *                 High or Low part of the register.
         * @param SetValue Pointer to the register's member function that will store the result
         *                 into the High or Low part of the register.
         * 
         * @note The Z flag is set to false.
         * @note The S flag is set to false.
         * @note The H flag is set to false.
         * @note The C flag is set to the shifted-out bit.
         * 
         * @return number of cycles.
         */
        size_t NoPrefixRotateLeftCarry(RegisterPointer reg, RegisterGet8Bit GetValue, RegisterSet8Bit SetValue);

        /**
         * @note Non prefixed version of the rotate instruction, difference being the 
         *       cycle count and zero flag not being set compared to the prefixed one.
         * 
         * @brief Rotates the given register to the right by 1 through the carry flag, that is,
         *        appending the state of the carry flag bit to the left of the same register. 
         *        The shifted-out bit will become the new state of the carry flag.
         * 
         * @param reg      Pointer to the register that will be used as the operand.
         * @param GetValue Pointer to the register's member function that will take the
         *                 High or Low part of the register.
         * @param SetValue Pointer to the register's member function that will store the result
         *                 into the High or Low part of the register.
         * 
         * @note The Z flag is set to false.
         * @note The S flag is set to false.
         * @note The H flag is set to false.
         * @note The C flag is set to the shifted-out bit.
         * 
         * @return number of cycles.
         */
        size_t NoPrefixRotateRight(RegisterPointer reg, RegisterGet8Bit GetValue, RegisterSet8Bit SetValue);

        /**
         * @note Non prefixed version of the rotate instruction, difference being the 
         *       cycle count and zero flag not being set compared to the prefixed one.
         * 
         * @brief Rotates the given register to the right by 1,
         *        appending the shifted-out bit to the left of that same register
         *        and storing that same bit into the carry register.
         * 
         * @param reg      Pointer to the register that will be used as the operand.
         * @param GetValue Pointer to the register's member function that will take the
         *                 High or Low part of the register.
         * @param SetValue Pointer to the register's member function that will store the result
         *                 into the High or Low part of the register.
         * 
         * @note The Z flag is set to false.
         * @note The S flag is set to false.
         * @note The H flag is set to false.
         * @note The C flag is set to the shifted-out bit.
         * 
         * @return number of cycles.
         */
        size_t NoPrefixRotateRightCarry(RegisterPointer reg, RegisterGet8Bit GetValue, RegisterSet8Bit SetValue);

        /**                                     Prefixed Instructions                                     **/
    

        /**
         * @brief Rotates the given register's to the left by 1 through the carry flag, that is,
         *        appending the state of the carry flag bit to the right of the same register.
         *        The shifted-out bit will become the new state of the carry flag.
         * 
         * @param reg      Pointer to the register that will be used as the operand.
         * @param GetValue Pointer to the register's member function that will take the
         *                 High or Low part of the register.
         * @param SetValue Pointer to the register's member function that will store the result
         *                 into the High or Low part of the register.
         * 
         * @note The Z flag is set according to the result.
         * @note The S flag is set to false.
         * @note The H flag is set to false.
         * @note The C flag is set to the shifted-out bit.
         * 
         * @return number of cycles.
         */
        size_t RotateLeft(RegisterPointer reg, RegisterGet8Bit GetValue, RegisterSet8Bit SetValue);

        /**
         * @brief Rotates the given register to the left by 1,
         *        appending the shifted-out bit to the right of that same register
         *        and storing that same bit into the carry register.
         * 
         * @param reg      Pointer to the register that will be used as the operand.
         * @param GetValue Pointer to the register's member function that will take the
         *                 High or Low part of the register.
         * @param SetValue Pointer to the register's member function that will store the result
         *                 into the High or Low part of the register.
         * 
         * @note The Z flag is set to according to the result.
         * @note The S flag is set to false.
         * @note The H flag is set to false.
         * @note The C flag is set to the shifted-out bit.
         * 
         * @return number of cycles.
         */
        size_t RotateLeftCarry(RegisterPointer reg, RegisterGet8Bit GetValue, RegisterSet8Bit SetValue);

        /**
         * @brief Rotates the value pointed to by addrReg to the left by 1,
         *        appending the shifted-out bit to the right of that same value
         *        and storing that same bit into the carry register.
         * 
         * @param addrReg Pointer to the register containing the address of the value
         *                that will be used as the operand.
         * 
         * @note The Z flag is set to according to the result.
         * @note The S flag is set to false.
         * @note The H flag is set to false.
         * @note The C flag is set to the shifted-out bit.
         * 
         * @return number of cycles.
         */
        size_t RotateLeftCarry_Addr(RegisterPointer addrReg);

        /**
         * @brief Rotates the given register to the right by 1 through the carry flag, that is,
         *        appending the state of the carry flag bit to the left of the same register. 
         *        The shifted-out bit will become the new state of the carry flag.
         * 
         * @param reg      Pointer to the register that will be used as the operand.
         * @param GetValue Pointer to the register's member function that will take the
         *                 High or Low part of the register.
         * @param SetValue Pointer to the register's member function that will store the result
         *                 into the High or Low part of the register.
         * 
         * @note The Z flag is set to according to the result.
         * @note The S flag is set to false.
         * @note The H flag is set to false.
         * @note The C flag is set to the shifted-out bit.
         * 
         * @return number of cycles.
         */
        size_t RotateRight(RegisterPointer reg, RegisterGet8Bit GetValue, RegisterSet8Bit SetValue);

        /**
         * @brief Rotates the given register to the right by 1,
         *        appending the shifted-out bit to the left of that same register
         *        and storing that same bit into the carry register.
         * 
         * @param reg      Pointer to the register that will be used as the operand.
         * @param GetValue Pointer to the register's member function that will take the
         *                 High or Low part of the register.
         * @param SetValue Pointer to the register's member function that will store the result
         *                 into the High or Low part of the register.
         * 
         * @note The Z flag is set to according to the result.
         * @note The S flag is set to false.
         * @note The H flag is set to false.
         * @note The C flag is set to the shifted-out bit.
         * 
         * @return number of cycles.
         */
        size_t RotateRightCarry(RegisterPointer reg, RegisterGet8Bit GetValue, RegisterSet8Bit SetValue);

    
    
    };

}
