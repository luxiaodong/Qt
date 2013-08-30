/*  DecMPA - simple MPEG Audio decoding library.
    Copyright (C) 2002  Hauke Duden

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

	For more information look at the file License.txt in this package.

	email: hazard_hd@users.sourceforge.net
*/

#ifndef _DECODEFILE_CPP_
#define _DECODEFILE_CPP_
#include "include/decmpa.h"

#include <stdio.h>
#include <memory.h>


//This example demonstrates how to perform simple file decoding wit hDecMPA
//The input data is read from an MP3 file, then decoded and then
//written to an output file in raw 16bit PCM format .

//The output file does not have a specific format (like, for example, WAV).
//It only contains the raw data, as it might be passed to the soundcard
//for playback.

//bool MainDecode(const char* sInFilename,FILE* pOutFile);
/*

int main(int nArgCount,char* aArgs[])
{
	bool bResult=false;

	printf("\n");

	//make sure that the right number of arguments was passed
	if(nArgCount<2 || nArgCount>3)
	{
		printf(	"Usage: decodefile INFILE [OUTFILE]\n\n"
				"If OUTFILE is not specified, 'out.raw' will be used.\n\n");
	}
	else
	{
		FILE* pOutFile;
		const char* sInFilename=aArgs[1];
		const char* sOutFilename=(nArgCount>=3) ? aArgs[2] : "out.raw";
		
		//open the output file
		pOutFile=fopen(sOutFilename,"wb");
		if(pOutFile==NULL)
			printf("Error opening output file.\n");
		else
		{
			printf("decoding '%s'...\n",sInFilename);

			//decode the MP3 data from the input file and
			//write it to the output file
			if(Decode(sInFilename,pOutFile))
			{
				printf("'%s' successfully written.\n",sOutFilename);
				bResult=true;
			}

			fclose(pOutFile);
		}
	}

	return bResult ? 0 : 1;
}
*/

//the main decoding function
/*
bool MainDecode(const char* sInFile,FILE* pOutFile)
{
	bool bResult=false;
	
	//the purpose of this function is to decode MP3 data from sInFile and
	//write the raw, decoded PCM data to pOutFile

	//create the decoder object
	void* pDecoder;
	
	if(DecMPA_CreateUsingFile(&pDecoder,sInFile,DECMPA_VERSION)!=DECMPA_OK)
		printf("Error creating decoder.\n");
	else
	{
		int DecodeResult;
		long nDecodedBytes;
		char* pOutBuffer;
		long nOutBufferBytes;
		bool bFirstDecode=true;

		//allocate an output buffer to store the decoded data.
		//the size can be chosen freely, DecMPA will work with what you provide
		nOutBufferBytes=16384;
		pOutBuffer=new char[nOutBufferBytes];

		//now just decode the data until the end of the input file is reached
		while(true)
		{
			DecodeResult=DecMPA_Decode(pDecoder,pOutBuffer,nOutBufferBytes,&nDecodedBytes);			
			if(DecodeResult==DECMPA_OK)
			{
				if(bFirstDecode)
				{
					long Duration=-1;
					DecMPA_MPEGHeader Header;

					//print some information about the stream

					memset(&Header,0,sizeof(Header));

					DecMPA_GetDuration(pDecoder,&Duration);
					DecMPA_GetMPEGHeader(pDecoder,&Header);

					printf(	"duration:   %ld ms\n"
							"layer:      %d\n"
							"bitrate:    %d kbps\n"
							"frequency:  %d Hz\n",Duration,Header.nLayer,Header.nBitRateKbps,Header.nFrequency);

					bFirstDecode=false;
				}

				//some data was decoded - write it to the output file
				//the format of the decoded data (i.e. the frequency and
				//wether it is stereo) could be obtained by calling
				//DecMPA_GetOutputFormat. We just write the data to a
				//file here, though.
				if(fwrite(pOutBuffer,1,nDecodedBytes,pOutFile)!=(size_t)nDecodedBytes)
				{
					printf("Error writing to output file");
					break;
				}
			}
			else
			{
				//the decoder was unable to decode more data
				if(DecodeResult==DECMPA_END)
				{
					//everything is ok, the end of the file was reached
					bResult=true;
				}
				else
				{
					//some error occurred
					printf("Error during decoding.\n");
				}

				//always stop the decoding loop here, no matter wether an
				//error occurred or not,
				break;
			}
		}

		//free the output buffer
		delete[] pOutBuffer;

		//destroy the decoder
		DecMPA_Destroy(pDecoder);
	}

	return bResult;
}
*/
#endif
