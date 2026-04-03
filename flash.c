#include "flash.h"
#include "uart.h"
void ghi_flash(void){
	//mo khoa flash
	FLASH_UnlockBank1();
	/* Clear All pending flags */
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);	
	//xoa truoc khi ghi
	FLASH_ErasePage(START_PAGE_60);
	//ghi vao 
	FLASH_ProgramWord(PhanNguyen_ADDR , PhanNguyen);
	FLASH_ProgramWord(PhanThapPhan_ADDR , PhanThapPhan);
	FLASH_ProgramWord(HeSo_ADDR , HeSo);
	//khoa lai
	FLASH_LockBank1();
	printf("Ghi Xong\n");
}
void doc_flash(void){
	
	PhanNguyen =*(__IO uint32_t*)PhanNguyen_ADDR; 
	PhanThapPhan = *(__IO uint32_t*)PhanThapPhan_ADDR;
	HeSo = *(__IO uint32_t*)HeSo_ADDR;
}
