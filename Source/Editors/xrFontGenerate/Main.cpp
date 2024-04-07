// StalkerFontGenerate.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include "..\..\BearBundle\BearCore\BearCore.hpp"
#include "..\..\BearBundle\BearGraphics\BearGraphics.hpp"
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_OUTLINE_H
#include FT_BITMAP_H
bool shoc_format = false;
bool GenerateFont(const bchar* file, bsize size, bsize&real_size, bsize width, bsize height, BearImage& Image,BearVector< BearVector3<float>>&OutVector)
{
	const bsize chars_size = 256;
	BearImage ImageList[256];
	BearVector2<float> SizeList[256];
	{
		BearFileStream font_file;
		if (!font_file.Open(file))
			return false;

		BearMemoryStream font(font_file);

		FT_Library library = 0;
		FT_Face     face = 0;
		if (FT_Init_FreeType(&library))
		{
			FT_Done_FreeType(library);
			return false;
		}
		if (FT_New_Memory_Face(library, reinterpret_cast<const FT_Byte*>(font.Begin()), static_cast<FT_Long>(font.Size()), 0, &face))
		{
			FT_Done_Face(face);
			FT_Done_FreeType(library);
			return false;
		}

		FT_Select_Charmap(face, FT_ENCODING_UNICODE);
		FT_Set_Char_Size(face, 0, static_cast<FT_F26Dot6>(size * 64), static_cast<FT_UInt>(width), static_cast<FT_UInt>(height));
		FT_Set_Pixel_Sizes(face, 0, static_cast<FT_UInt>(size));;

		
		OutVector.resize(chars_size);

		real_size = static_cast<bsize>(face->size->metrics.height) / static_cast<bsize>(1 << 6);
		for (bsize i = 32; i < chars_size; i++) {

			bchar16 id = chars_size == i + 1 ? L' ' : BearEncoding::ToUTF16((char)i);
			if (id == L'\r' || id == L'\n' || id == L'	')continue;
			if (FT_Load_Char(face, id, FT_LOAD_TARGET_NORMAL | FT_LOAD_FORCE_AUTOHINT | FT_LOAD_RENDER) != 0)
			{
				continue;
			}
			FT_Glyph glyphDesc;
			FT_Get_Glyph(face->glyph, &glyphDesc);

			FT_Glyph_To_Bitmap(&glyphDesc, FT_RENDER_MODE_NORMAL, 0, 1);
			FT_Bitmap& bitmap = reinterpret_cast<FT_BitmapGlyph>(glyphDesc)->bitmap;
			unsigned char* data = bitmap.buffer;
			uint32 width_char = bitmap.width;
			uint32 height_char = bitmap.rows;
			if (width_char == 0 || height_char == 0)continue;

			
			if (shoc_format)
				ImageList[i].Create(width_char, height_char, 1, 1, BearTexturePixelFormat::R8G8B8A8);
			else
				ImageList[i].Create(width_char, height_char, 1, 1, BearTexturePixelFormat::R8);
			if (shoc_format)
				ImageList[i].Fill(BearColor::Transparent);
			else
				ImageList[i].Fill(BearColor::Black);


			if (shoc_format)
			{
				for (uint32 y = 0; y < height_char; ++y)
				{
					for (uint32 x = 0; x < width_char; ++x)
					{
						{
							bsize index = static_cast<bsize> (4 * (x + y * width_char));
							((uint8*)*ImageList[i])[index] = 255;
							((uint8*)*ImageList[i])[index + 1] = 255;
							((uint8*)*ImageList[i])[index + 2] = 255;
						}
						bsize index = static_cast<bsize> (4 * (x + y * width_char) + 3);
						((uint8*)*ImageList[i])[index] = data[x];
					}
					data += bitmap.pitch;
				}

			}
			else
			{
				for (uint32 y = 0; y < height_char; ++y)
				{
					for (uint32 x = 0; x < width_char; ++x)
					{

						bsize index = static_cast<bsize> (x + y * width_char);
						((uint8*)*ImageList[i])[index] = data[x];
					}
					data += bitmap.pitch;
				}

			}
			SizeList[i].set(static_cast<float>(face->glyph->metrics.horiAdvance) / static_cast<float>(1 << 6), (static_cast<bint>(face->glyph->metrics.horiBearingY) / static_cast<bint>(1 << 6)));
			
		}
		FT_Done_Face(face);
		FT_Done_FreeType(library);
	}
	{
		bsize ShiftY = real_size - size;
		for (bsize i = 0; i < chars_size; i++)
		{
			SizeList[i].y = size - (SizeList[i].y+ ShiftY);
		}
	
	}
	{
		uint32 size_texture = static_cast<uint32>(bear_recommended_size(static_cast<bsize>(sqrtf(static_cast<float>((chars_size - 32) * (size ) * (real_size))))));

		Image.Create(size_texture, size_texture, 1, 1, BearTexturePixelFormat::R8G8B8A8);
		if (shoc_format)
			Image.Fill(BearColor::Transparent);
		else
			Image.Fill(BearColor::Black);
		bsize x_t = 0, y_t = 0;
		for (bsize i = 32; i < chars_size; i++)
		{
			if (ImageList[i].Empty())continue;
			if (x_t + ImageList[i].GetSize().x + 4 > size_texture)
			{
				x_t = 0; y_t += real_size;
			}
			Image.Append(x_t, y_t +static_cast<bsize>( SizeList[i].y), ImageList[i], 0, 0);


			OutVector[i].set(static_cast<float>(x_t), static_cast<float>(y_t), SizeList[i].x+ static_cast<float>(x_t));
			x_t += ImageList[i].GetSize().x + 4;
		}
	}

	Image.GenerateMipmap();
	return true;
}
void GenerateFont(const bchar* name,uint32 height=16)
{
	bsize RealSize;
	{
		BearStringPath NameDDS,NameINI;
		BearString::Copy(NameDDS, name);
		if (BearString::ToCharWithEnd(NameDDS, '.'))
		{
			BearString::ToCharWithEnd(NameDDS, '.')[0] = 0;
		}
		BearString::Contact(NameDDS, "_800.dds");
		BearString::Copy(NameINI, name);
		if (BearString::ToCharWithEnd(NameINI, '.'))
		{
			BearString::ToCharWithEnd(NameINI, '.')[0] = 0;
		}
		BearString::Contact(NameINI, "_800.ini");

		BearImage img;
		BearVector< BearVector3<float>> OutVector;
		if (GenerateFont(name, height, RealSize,800, 600, img, OutVector))
		{
			if (shoc_format)
			{
				img.Convert(BearTexturePixelFormat::BC3);
			}
			img.SaveToDds(NameDDS);
			BearString INI;
			INI.append(TEXT("[symbol_coords]\n"));
			INI.append_printf(TEXT("height=%d\n"), uint32(RealSize));
			for (int i = 0; i < 256; i++)
			{
				INI.append_printf(TEXT("%03d = %f,%f,%f\n"), i, OutVector[i].x, OutVector[i].y-1, OutVector[i].z);
			}
			BearFileStream FileINI;
			if (FileINI.Open(NameINI, FileINI.M_Write))
			{
				FileINI.WriteString(INI, BearEncoding::ANSI, false);
				FileINI.Close();
			}
		}
		else
		{
			BearCore::GetLog()->Printf(TEXT("Error:Genarate %s[%d]"), name, 800);
		}
		
	}
	{
		BearStringPath NameDDS, NameINI;

		BearString::Copy(NameDDS, name);
		if (BearString::ToCharWithEnd(NameDDS, '.'))
		{
			BearString::ToCharWithEnd(NameDDS, '.')[0] = 0;
		}
		BearString::Contact(NameDDS, "_1024.dds");
		BearString::Copy(NameINI, name);
		if (BearString::ToCharWithEnd(NameINI, '.'))
		{
			BearString::ToCharWithEnd(NameINI, '.')[0] = 0;
		}
		BearString::Contact(NameINI, "_1024.ini");

		BearImage img;
		BearVector< BearVector3<float>> OutVector;
		if (GenerateFont(name, height, RealSize, 1024, 768, img, OutVector))
		{
			if (shoc_format)
			{
				img.Convert(BearTexturePixelFormat::BC3);
			}
			img.SaveToDds(NameDDS);
			BearString INI;
			INI.append(TEXT("[symbol_coords]\n"));
			INI.append_printf(TEXT("height=%d\n"), uint32(RealSize ));
			for (int i = 0; i < 256; i++)
			{
				INI.append_printf(TEXT("%03d = %f,%f,%f\n"), i, OutVector[i].x, OutVector[i].y-1, OutVector[i].z);
			}
			BearFileStream FileINI;
			if (FileINI.Open(NameINI, FileINI.M_Write))
			{
				FileINI.WriteString(INI, BearEncoding::ANSI, false);
				FileINI.Close();
			}
		}
		else
		{
			BearCore::GetLog()->Printf(TEXT("Error:Genarate %s[%d]"), name, 1024);
		}
	}
	{
		BearStringPath NameDDS, NameINI;

		BearString::Copy(NameDDS, name);
		if (BearString::ToCharWithEnd(NameDDS, '.'))
		{
			BearString::ToCharWithEnd(NameDDS, '.')[0] = 0;
		}
		BearString::Contact(NameDDS, "_1600.dds");
		BearString::Copy(NameINI, name);
		if (BearString::ToCharWithEnd(NameINI, '.'))
		{
			BearString::ToCharWithEnd(NameINI, '.')[0] = 0;
		}
		BearString::Contact(NameINI, "_1600.ini");

		BearImage img;
		BearVector< BearVector3<float>> OutVector;
		if (GenerateFont(name, height, RealSize, 1600, 900, img, OutVector))
		{
			if (shoc_format)
			{
				img.Convert(BearTexturePixelFormat::BC3);
			}
			img.SaveToDds(NameDDS);
			BearString INI;
			INI.append(TEXT("[symbol_coords]\n"));
			INI.append_printf(TEXT("height=%d\n"), uint32(RealSize));
			for (int i = 0; i < 256; i++)
			{
				INI.append_printf(TEXT("%03d = %f,%f,%f\n"), i, OutVector[i].x, OutVector[i].y-1, OutVector[i].z);
			}
			BearFileStream FileINI;
			if (FileINI.Open(NameINI, FileINI.M_Write))
			{
				FileINI.WriteString(INI, BearEncoding::ANSI, false);
				FileINI.Close();
			}
		}
		else
		{
			BearCore::GetLog()->Printf(TEXT("Error:Genarate %s[%d]"), name, 1600);
		}
	}
}
int main(int argc, char* argv[])
{
	if (argc == 0)return -1;
	BearCore::Initialize();
	{
		uint32 Height = 24;
		if(argc>2)
		for (int i = 0; i < argc - 2; i++)
		{
			if (BearString::CompareWithoutCase(argv[i], "-height")==0)
			{
				if (BearString::Scanf(argv[i + 1], "%d", &Height) == 1)
				{
					if (Height < 8)Height = 8;
				}
			
			}
			if (BearString::CompareWithoutCase(argv[i], "-shoc") == 0)
			{
				shoc_format = true;

			}
		}
		BearStringPath Name;
		BearString::Copy(Name, argv[argc - 1]);
		if (!BearFileManager::FileExists(Name))
		{
			BearCore::GetLog()->Printf(TEXT("Error:No Found %s"), Name);
		}
		else
		{
			GenerateFont(Name, Height);
		}
	}
	BearCore::Destroy();
	return 0;
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
