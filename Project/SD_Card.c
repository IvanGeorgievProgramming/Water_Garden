#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include <SD_Card.h>
#include <dispcolor.h>
#include <diskio.h>
#include <string.h>
#include <ff.h>
#include <font.h>
#include <disp_common.h>
#include <button.h>
#include <timers.h>
#include <delay.h>
#include <time.h>
#include <SD_Card.h>
#include <main.h>


Data_Water_Expense_t Data_Water_Expense;

uint32_t imageOffset;
char array[60];

////////////////////////////////////////////////////////////////////////////////
void write_struct_in_sd_card(){
    
    
}

////////////////////////////////////////////////////////////////////////////////
void Save_data_water_expense(char file_name[50]){
    FRESULT res;
    FIL file;
//    unsigned int len;

    res = f_open(&file, file_name, FA_READ | FA_WRITE | FA_OPEN_APPEND );
    if (res != FR_OK)  return;


    
}

////////////////////////////////////////////////////////////////////////////////
void Write_to_file(const char* fname)
{
  FRESULT res;
  FIL file;
  unsigned int len;
  RTC_DateTimeTypeDef *p = &RTC_DateTime;
 
  
  res = f_open(&file, fname, FA_READ | FA_WRITE | FA_OPEN_APPEND );
  if (res != FR_OK)  return;

//  unsigned int bytesRead;
  uint8_t header[54];
  
  res = f_lseek(&file, 0x00);
  if(res != FR_OK){
    f_close(&file);
    return;
  }

  res = f_read(&file, header, sizeof(header), &len);
  if (res != FR_OK){
    f_close(&file);
    return;
  }

  if ((header[0] != 0x55) || (header[1] != 0x55)){
    f_close(&file);
    return;
  }

//  uint32_t DataOffset = header[10] | (header[11] << 8) | (header[12] << 16) | (header[13] << 24);
  uint32_t FirstEmptyOffset = header[14] | (header[15] << 8) | (header[16] << 16) | (header[17] << 24);

  res = f_lseek(&file, FirstEmptyOffset);
  if(res != FR_OK){
    f_close(&file);
    return;
  }

  res = f_write(&file, p, sizeof(RTC_DateTimeTypeDef), &len);
  if (res != FR_OK)  return;

  FirstEmptyOffset += sizeof(RTC_DateTimeTypeDef);
  header[14]= (uint8_t) FirstEmptyOffset;
  FirstEmptyOffset >>= 8;
  header[15]= (uint8_t) FirstEmptyOffset;
  FirstEmptyOffset >>= 8;
  header[16]= (uint8_t) FirstEmptyOffset;
  FirstEmptyOffset >>= 8;
  header[17]= (uint8_t) FirstEmptyOffset;

  res = f_lseek(&file, 0x00);
  if(res != FR_OK){
    f_close(&file);
    return;
  }

  res = f_write(&file, header, sizeof(header), &len);
  if (res != FR_OK)  return;
  
  f_close(&file);
  delay_ms(1000);

  res = f_sync(&file);
  if (res != FR_OK)  return;
  delay_ms(1000);
}

/*
  // Подготавливаем буфер строки картинки для вывода
//  uint8_t imageRow[(240 * 3 + 3) & ~3];
  uint8_t TextBuff[240];

//  for (uint32_t y = 0; y < imageHeight; y++)
  for (uint32_t y = 0; y < 2; y++)
  {
    res = f_read(&file, TextBuff, 240, &bytesRead);
    if (res != FR_OK)
    {
      f_close(&file);
      return;
    }
      
    uint32_t textIdx = 0;
    char h[17];

    for (int i= 0; i<sizeof(h)-1; i++)
    {     
     int TEMP= TextBuff[textIdx++];
     h[i] = TEMP;
     h[i] = h[i] >> 4;
      if (h[i] < 0x0a) 
        h[i] = h[i] + 0x30;
      if (h[i] >0x09 && h[i] < 0x10) 
        h[i] = h[i] + 0x37;
 
     i++;

     h[i] = TEMP & 0x0f;
       if (h[i] < 0x0a) 
        h[i] = h[i] + 0x30;
       if (h[i] >0x09 && h[i] < 0x10) 
        h[i] = h[i] + 0x37;
     
    }
    h[17]= 0;
    dispcolor_printf(10, 70, FONTID_16F, RGB565(255, 255, 255), TextBuff );
    delay_ms(2000);
    dispcolor_ClearScreen();
    
   
    
    
    for (uint32_t x = 0; x < imageWidth; x++)
    {
      uint8_t b = imageRow[rowIdx++];
      uint8_t g = imageRow[rowIdx++];
      uint8_t r = imageRow[rowIdx++];
      PixBuff[x] = RGB565(r, g, b);
    }

    dispcolor_DrawPartXY(0, imageHeight - y - 1, imageWidth, 1, PixBuff);
*/

//==============================================================================
void Test_NewFile2(const char *fname)
{
  FRESULT res;
  FIL file;
//  unsigned int len;
  
  uint8_t header[54];
//  char str[]="Hello ma nibba how you doinhelloh";
//  char str2[]="pedal";
  
  for(int i=0; i<sizeof(header); i++)   header[i]= 0;
  
  header[0]= 0x55;
  header[1]= 0x55;
  header[4]= 0x36;
  

  res = f_open(&file, fname, FA_WRITE | FA_OPEN_ALWAYS );
  if (res != FR_OK)  return;
    
  f_close(&file);
  
  res = f_sync(&file);
  if (res != FR_OK)  return;
}
/*  res = f_open(&file, fname, FA_READ);
  if (res != FR_OK)      return;
  sdread_SD(file, array[5], p, len);  
  res = f_close(&file);  
  if(res != FR_OK)      return;*/
////////////////////////////////////////////////////////////////////////////////
/*  DIR DirInfo;
  FILINFO FileInfo;

  res = f_chdir("/Data");
  if (res != FR_OK)
    return;

  res = f_opendir(&DirInfo, "");
  if (res != FR_OK)
    return;

    res = f_readdir(&DirInfo, &FileInfo);
    if (res != FR_OK)
    return;

    res = f_open(&file, fname, FA_WRITE | FA_OPEN_ALWAYS);
  if (res != FR_OK)
    return;
  
  res = f_write(&file, str2, sizeof(str2), &len);
  if (res != FR_OK)
    return;

 f_close(&file);

 res = f_sync(&file);
  if (res != FR_OK)
    return;
*/  

//==============================================================================
void Save_Data(void)
{
  FATFS fatfs;
  DIR DirInfo;
  FILINFO FileInfo;
  FRESULT res;

  res = f_mount(&fatfs, "0", 1);
  if (res != FR_OK)  return;
  
  res = f_chdir("/Data");
  if (res != FR_OK)  return;

  res = f_opendir(&DirInfo, "");
  if (res != FR_OK)  return;

  while (1)
  {
    res = f_readdir(&DirInfo, &FileInfo);
    if (res != FR_OK)  break;
    
    if (FileInfo.fname[0] == 0)  break;
      
    char *pExt = strstr(FileInfo.fname, ".dat");
    if (!pExt)  pExt = strstr(FileInfo.fname, ".DAT");    
    if (pExt){
      dispcolor_printf(80, 150, FONTID_16F, RGB565(255, 200, 200), FileInfo.fname);
      //Write_to_file("WM1.dat");
      Save_data_water_expense("WM1.dat");
      //Test_NewFile2("WM1.dat");
    }   
  }
}

//==============================================================================