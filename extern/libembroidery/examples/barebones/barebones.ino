//libembroidery barebones example
//TODO: This is a work in progress sketch using libembroidery
//TODO: NOTE: you may need to modify the SD_CS and/or supply your own sample file

#include <SD.h>
#include "emb-pattern.h"
#include "geom-arc.h" //TODO: remove this later

#define SD_CS 5 // Card select

//SPCR States (SPI Control Register)
uint8_t SPI_SDCARD = 0; // Set the SPCR to this before reading

uint32_t stitchCount = 0;

void setup(void)
{
    Serial.begin(9600);
    Serial.println(F("libembroidery barebones example"));

    Serial.print(F("Initializing SD card..."));
    if(!SD.begin(SD_CS))
    {
        Serial.println(F("failed!"));
        return;
    }
    Serial.println(F("OK!"));
    SPI_SDCARD = SPCR; //Save the state for later use
}

void loop(void)
{
    loadEmb("sample.exp");
}

void loadEmb(char* fileName)
{
    SPCR = SPI_SDCARD;

    if(!SD.exists(fileName))
    {
        Serial.println("file DOES NOT exist!.");
        return;
    }

    //==============================
    // Reading
    //==============================
    EmbPattern* p = 0;
    int successful = 0;

    p = embPattern_create();
    if(!p) { Serial.println(F("ERROR: Could not allocate memory for embroidery pattern.")); return; }

    successful = embPattern_read(p, fileName);
    if(!successful)
    {
        Serial.print("ERROR: Reading file was unsuccessful: ");
        Serial.println(fileName);
        embPattern_free(p);
        return;
    }

    //==============================
    // Report Design Info
    //==============================
    Serial.print(F("Total Stitches: "));
    Serial.println(stitchCount, DEC); //TODO: For some reason we are missing the last 2 stitches. Investigate embPattern_addStitchAbs() causing a small memory leak.
    stitchCount = 0;

    //==============================
    // Cleanup
    //==============================
    embPattern_free(p);
}

void eventHandler_addStitchAbs(EmbPattern* p, double x, double y, int flags, int isAutoColorIndex)
{
    stitchCount++;
    Serial.print(F("Adding Stitch: "));
    Serial.println(stitchCount, DEC);
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
