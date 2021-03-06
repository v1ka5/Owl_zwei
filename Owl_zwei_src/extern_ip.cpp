/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *   Owl Zwei - Remote Administration Toolkit For Windows                    *
 *                                                                           * 
 *   Copyright 2010 Yati Sagade                                              *
 *                                                                           *
 *   This file is part of Owl Zwei.                                          *
 *                                                                           *
 *   Owl Zwei is free software: you can redistribute it and/or modify        *
 *   it under the terms of the GNU General Public License as published by    *
 *   the Free Software Foundation, either version 3 of the License, or       *
 *   (at your option) any later version.                                     *
 *                                                                           *
 *   Owl Zwei is distributed in the hope that it will be useful,             *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 *   GNU General Public License for more details.                            *
 *                                                                           *
 *   You should have received a copy of the GNU General Public License       *
 *   along with Owl Zwei.  If not, see <http://www.gnu.org/licenses/>.       *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
//copied in entirety from Beatrix, sans the Italian comments :)
#include "extern_ip.h"

char *GetExternalIp(){
	MB("GEIP Called");
	SOCKET s = connect_to(WEBSERVER,WEBPORT);

	if (s<=0) return "0.0.0.0\0";
	//MB("still there, connected.");
	char *pRequest = new char[1024];
	//MB("new done");
	wsprintf(pRequest,"GET / HTTP/1.1\nHost: %s:%d\n\n\0",WEBSERVER,WEBPORT);
	//MB("wsprintf over");
	SendLn(s,pRequest);
	//MB("request sent");
	char *pBuffer = new char[0xFFF];
	int *iIpDigits = new int[3];
	int res;

	do{
		//MB("do");
		res = recv(s,pBuffer,MAX_BUF_LEN,0);
		pBuffer[res] = 0;
		if (strstr(pBuffer,"</TITLE>") != NULL){
 			pBuffer = ExtractIpFromResponse(pBuffer);
			break;
		}
	}while(res >= 0);

	closesocket(s);
	return pBuffer;
}

char *ExtractIpFromResponse(char *pResponse){
	//MB("EIPFR called");
	char *pIp = new char[0x10];
	memset(pIp,'\0',0x10);

	int iDotsCount = 0;
	bool bDigitFound = false;

	for (register int c = 0; c<lstrlen(pResponse) && iDotsCount < 4; c++){
		if (isdigit(pResponse[c])){
			wsprintf(pIp,"%s%c",pIp,pResponse[c]);	
			bDigitFound = true;


		}else if(pResponse[c] == '.' && bDigitFound){
			wsprintf(pIp,"%s.",pIp);				
			iDotsCount++;
		}
	}

	return pIp;
}
