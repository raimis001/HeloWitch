#include "arduino.h"
#include "SoftwareSerial.h"

class MP3Player
{
    public:
        bool sleeping = false;

        MP3Player(int rxPin, int txPin)
        {
            _serial = new SoftwareSerial(rxPin, txPin);
            _serial->begin(9600);
        }

        ~MP3Player();

        void reset()
        {
            execute_CMD(Mp3_Commands_Reset);
        }
        void sleep()
        {
            execute_CMD(Mp3_Commands_Sleep);
            sleeping = true;
        }
        void awake()
        {
            execute_CMD(Mp3_Commands_Awake);
            sleeping = false;
        }
        void play()
        {
            execute_CMD(Mp3_Commands_Start,5);
        }
        void pause()
        {
            execute_CMD(Mp3_Commands_Pause);
        }
        void stop()
        {
            execute_CMD(Mp3_Commands_Stop);
        }
        void next()
        {
            execute_CMD(Mp3_Commands_PlayNextTrack);
        }
        void previous()
        {
            execute_CMD(Mp3_Commands_PlayPrevTrack);
        }
        void setVolume(byte volume) // 0-30
        {
            volume = constrain(volume, 0, 30);
            execute_CMD(Mp3_Commands_SetVolume, volume);
        }
    private:
        SoftwareSerial* _serial;

        void execute_CMD(byte CMD, byte Par2 = 0, byte ack = 0)
        {
            byte Par1 = 0;
            byte command[10] = { 
                START_BYTE, 
                VERSION_BYTE, 
                COMMAND_LENGHT, 
                CMD, 
                ack, 
                Par1, 
                Par2, 
                0, 
                0, 
                END_BYTE 
            };
            unsigned int checksum = 0 - (VERSION_BYTE + COMMAND_LENGHT + CMD + ack + Par1 + Par2);
            command[7] = (byte)(checksum >> 8);
            command[8] = (byte)(checksum & 0xFF);
            for (int i = 0; i < 10; i++)
            {
                _serial->write(command[i]);
            }
        }
    //protected:
        // command structure
        const byte START_BYTE = 0x7E;
        const byte VERSION_BYTE = 0xFF;
        const byte COMMAND_LENGHT = 0x06;
        const byte END_BYTE = 0xEF;
        //const byte Acknowledge = 0x00; //Returns info with command 0x41 [0x01: info, 0x00: no info]

        const byte ackYes  = 0x01;
        const byte ackNo   = 0x00;

        enum Mp3_Commands
        {
            Mp3_Commands_None = 0x00,
            Mp3_Commands_PlayNextTrack = 0x01,
            Mp3_Commands_PlayPrevTrack = 0x02,
            Mp3_Commands_PlayGlobalTrack = 0x03,
            Mp3_Commands_IncVolume = 0x04,
            Mp3_Commands_DecVolume = 0x05,
            Mp3_Commands_SetVolume = 0x06,
            Mp3_Commands_SetEq = 0x07,
            Mp3_Commands_LoopGlobalTrack = 0x08,
            Mp3_Commands_SetPlaybackMode = 0x08,
            Mp3_Commands_SetPlaybackSource = 0x09,
            Mp3_Commands_Sleep = 0x0a,
            Mp3_Commands_Awake = 0x0b,
            Mp3_Commands_Reset = 0x0c,
            Mp3_Commands_Start = 0x0d,
            Mp3_Commands_Pause = 0x0e,
            Mp3_Commands_PlayFolderTrack = 0x0f,
            Mp3_Commands_RepeatPlayInRoot = 0x11,
            Mp3_Commands_PlayMp3FolderTrack = 0x12,
            Mp3_Commands_PlayAdvertTrack = 0x13,
            Mp3_Commands_PlayFolderTrack16 = 0x14,
            Mp3_Commands_StopAdvert = 0x15,
            Mp3_Commands_Stop = 0x16,
            Mp3_Commands_LoopInFolder = 0x17,
            Mp3_Commands_PlayRandmomGlobalTrack = 0x18,
            Mp3_Commands_RepeatPlayCurrentTrack = 0x19,
            Mp3_Commands_SetDacInactive = 0x1a,
            Mp3_Commands_Requests = 0x30, // after this is all request, before all actions
            Mp3_Commands_GetPlaySources = 0x3f, // deprecated due to conflict with replies
            Mp3_Commands_GetStatus = 0x42,
            Mp3_Commands_GetVolume = 0x43,
            Mp3_Commands_GetEq = 0x44,
            Mp3_Commands_GetPlaybackMode = 0x45,
            Mp3_Commands_GetSoftwareVersion = 0x46,
            Mp3_Commands_GetUsbTrackCount = 0x47,
            Mp3_Commands_GetSdTrackCount = 0x48,
            Mp3_Commands_GetFlashTrackCount = 0x49,
            Mp3_Commands_GetUsbCurrentTrack = 0x4b,
            Mp3_Commands_GetSdCurrentTrack = 0x4c,
            Mp3_Commands_GetFlashCurrentTrack = 0x4d,
            Mp3_Commands_GetFolderTrackCount = 0x4e,
            Mp3_Commands_GetTotalFolderCount = 0x4f,
        };

};