#pragma once
#include <fstream>
#include <cstdint>
#include <chrono>
#include <random>

const unsigned int KEY_COUNT = 16;
const unsigned int MEMORY_SIZE = 4096;
const unsigned int REGISTER_COUNT = 16;
const unsigned int STACK_LEVELS = 16;
const unsigned int VIDEO_HEIGHT = 32;
const unsigned int VIDEO_WIDTH = 64;


class Chip8
{
public:
    // fix these later?

    uint8_t registers[REGISTER_COUNT]{};
    uint8_t memory[MEMORY_SIZE]{};
    uint16_t index{};
    uint16_t pc{};
    uint16_t stack[STACK_LEVELS]{};
    uint8_t sp{};
    uint8_t delayTimer{};
    uint8_t soundTimer{};
    uint8_t keypad[KEY_COUNT]{};
    uint32_t video[VIDEO_WIDTH * VIDEO_HEIGHT]{};
    uint16_t opcode{};

    // function pointer table setup
    typedef void(Chip8::*Chip8Func)();
    Chip8Func table[0xF + 1];
    Chip8Func table0[0xE + 1];
    Chip8Func table8[0xE + 1];
    Chip8Func tableE[0xE + 1];
    Chip8Func tableF[0x65 + 1];

    void Table0();
    void Table8();
    void TableE();
    void TableF();

    void OP_NULL();



    const unsigned int START_ADDRESS = 0x200;

    std::default_random_engine randGen;
    std::uniform_int_distribution<int> randByte;
    

    Chip8();

    void LoadROM(char const* filename);

    // OPCODES
    
    // CLS, clear display
    void OP_00E0();
    // RET, return from subroutine
    void OP_00EE();
    // JP addr, jump to location nnn
    void OP_1nnn();
    // CALL addr, call subroutine at nnn
    void OP_2nnn();
    // SE Vx, byte, skip next instruction if Vx = kk
    void OP_3xkk();
    // SNE Vx, byte, skip next instruction if Vx != kk
    void OP_4xkk();
    // SE Vx, Vy, skip next instruction if Vx = Vy
    void OP_5xy0();
    // LD Vx, byte, set Vx = kk
    void OP_6xkk();
    // ADD Vx, byte, set Vx = Vx + kk
    void OP_7xkk();
    // LD Vx, Vy, Set Vx = Vy
    void OP_8xy0();
    // OR Vx, Vy, set Vx OR Vy
    void OP_8xy1();
    // AND Vx, Yy, set Vx = Vx ADN Vy
    void OP_8xy2();
    // XOR Vx, Vy, Set Vx = Vx XOR Vy
    void OP_8xy3();
    // ADD Vx, Vy, set Vx + Vy, set VF = carry
    void OP_8xy4();
    // SUB Vx, Vy, set VF = NOT borrow
    void OP_8xy5();
    // SHR Vx, Set Vx = Vx SHR 1
    void OP_8xy6();
    // SUBN Vx, Vy, Set Vx = Vy - Vx, set VF = NOT borrow
    void OP_8xy7();
    // SHL Vx {,Vy}, Set Vx = VX SHL 1
    void OP_8xyE();
    // SNE Vx, Vy, skip next instruction if Vx != Vy
    void OP_9xy0();
    // Annn - LD I, addr, set I = nnn
    void OP_Annn();
    // Bnnn - JP V0, addr, jump to location nnn + V0
    void OP_Bnnn();
    // Cxkk - RND Vx, Byte, set Vx = random byte AND kk
    void OP_Cxkk();
    // Dxyn - DRW Vx, Vy, nibble, display n-byte sprite starting at mem location I at (Vx, Vy), set VF = collision
    void OP_Dxyn();
    // SKP Vx, skip next instruction if key with value of Vx is pressed
    void OP_Ex9E();
    // SKNP Vx, skip next instruction if key with value of Vx is not pressed
    void OP_ExA1();
    // LD Vx, DT, set Vx = delay timer value
    void OP_Fx07();
    // LD Vx, K, wait for key press, store value of key in Vx
    void OP_Fx0A();
    // LD DT, Vx, set delay timer = Vx
    void OP_Fx15();
    // LD St, Vx, set sound timer = Vx
    void OP_Fx18();
    // ADD I, Vx, set I = I + Vx
    void OP_Fx1E();
    // LD F, Vx, set I = location of sprite for digit Vx
    void OP_Fx29();
    // LD B, Vx, store BCD representation of Vx in mem locations I, I+1, and I+2
    void OP_Fx33();
    // LD [I], Vx, store registers V0 - Vx in memory starting at location I
    void OP_Fx55();
    // LD Vx, [I], read registers V0 - Vx from memory starting at location I
    void OP_Fx65();

    // FETCH, DECODE, EXECUTE

    void Cycle();
    
};