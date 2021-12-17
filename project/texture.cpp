#include "texture.h"

TexLoader::TexLoader(int n): texnum(n) {
	texture = new unsigned int[n];
}

void TexLoader::genTex() {
	glGenTextures(texnum, texture);
}

void TexLoader::bindTex(int i){
	glBindTexture(GL_TEXTURE_2D, texture[i]);
}

// �����ʾ�����飬������������ı�ʾ��
// ����: ͨ��ָ�룬����filename ָ����bitmap�ļ������ݡ�
// ͬʱҲ����bitmap��Ϣͷ.����֧��-bitλͼ��
unsigned char* TexLoader::loadBitmapFile(const char* filename, BITMAPINFOHEADER* bitmapInfoHeader){
	FILE* filePtr;	// �ļ�ָ��
	BITMAPFILEHEADER bitmapFileHeader;	// bitmap�ļ�ͷ
	unsigned char* bitmapImage;		// bitmapͼ������
	int	imageIdx = 0;		// ͼ��λ������
	unsigned char	tempRGB;	// ��������
								// �ԡ�������+����ģʽ���ļ�filename 
	fopen_s(&filePtr, filename, "rb");
	if (filePtr == NULL) {
		fprintf(stderr, "Error in LoadBitmapFile: file does not exist\n");
		return NULL;
	}
	// ����bitmap�ļ�ͼ
	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	// ��֤�Ƿ�Ϊbitmap�ļ�
	if (bitmapFileHeader.bfType != BITMAP_ID) {
		fprintf(stderr, "Error in LoadBitmapFile: the file is not a bitmap file\n");
		return NULL;
	}

	// ����bitmap��Ϣͷ
	fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
	// ���ļ�ָ������bitmap����
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);
	// Ϊװ��ͼ�����ݴ����㹻���ڴ�
	bitmapImage = new unsigned char[bitmapInfoHeader->biSizeImage];
	// ��֤�ڴ��Ƿ񴴽��ɹ�
	if (!bitmapImage) {
		fprintf(stderr, "Error in LoadBitmapFile: memory error\n");
		return NULL;
	}

	// ����bitmapͼ������
	fread(bitmapImage, 1, bitmapInfoHeader->biSizeImage, filePtr);
	// ȷ�϶���ɹ�
	if (bitmapImage == NULL) {
		fprintf(stderr, "Error in LoadBitmapFile: memory error\n");
		return NULL;
	}

	//����bitmap�б���ĸ�ʽ��BGR�����潻��R��B��ֵ���õ�RGB��ʽ
	for (imageIdx = 0;
		imageIdx < bitmapInfoHeader->biSizeImage; imageIdx += 3) {
		tempRGB = bitmapImage[imageIdx];
		bitmapImage[imageIdx] = bitmapImage[imageIdx + 2];
		bitmapImage[imageIdx + 2] = tempRGB;
	}
	// �ر�bitmapͼ���ļ�
	fclose(filePtr);
	return bitmapImage;
}

void TexLoader::loadTex(int i, const char* filename){

	BITMAPINFOHEADER bitmapInfoHeader;                                 // bitmap��Ϣͷ
	unsigned char* bitmapData;                                       // ��������

	bitmapData = loadBitmapFile(filename, &bitmapInfoHeader);
	glBindTexture(GL_TEXTURE_2D, texture[i]);
	// ָ����ǰ����ķŴ�/��С���˷�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//glTexImage2D(GL_TEXTURE_2D,
	//			 0, 	    //mipmap���(ͨ��Ϊ0����ʾ���ϲ�) 
	//			 GL_RGB,	//����ϣ���������к졢�̡�������
	//			 bitmapInfoHeader.biWidth, //��������������n�����б߿�+2 
	//			 bitmapInfoHeader.biHeight, //����߶ȣ�������n�����б߿�+2 
	//			 0, //�߿�(0=�ޱ߿�, 1=�б߿�) 
	//			 GL_RGB,	//bitmap���ݵĸ�ʽ
	//			 GL_UNSIGNED_BYTE, //ÿ����ɫ���ݵ�����
	//			 bitmapData);	//bitmap����ָ��  
	gluBuild2DMipmaps(GL_TEXTURE_2D,
					  GL_RGB,
					  bitmapInfoHeader.biWidth,
					  bitmapInfoHeader.biHeight,
					  GL_RGB,
					  GL_UNSIGNED_BYTE,
					  (const void*)bitmapData);
}