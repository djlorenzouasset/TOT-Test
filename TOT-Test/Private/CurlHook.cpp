#include "pch.h"
#include "Unreal.h"
#include "CurlHook.h"

bool CurlHook::FindCurlEasy()
{
    CUrlEasySetOptAddr = Memcury::Scanner::FindPattern(CUrlEasySetOptSig).Get();
    oCUrl_easy_setopt = decltype(oCUrl_easy_setopt)(CUrlEasySetOptAddr);

    if (!oCUrl_easy_setopt) 
    {
        return false;
    }

    std::cerr << std::format("Curl_easy_setopt() found at 0x{:X}\n", TO_ADDR(CUrlEasySetOptAddr));
    return true;
}

CURLcode CurlHook::CUrlHook(Curl_easy* Handle, CURLoption Option, va_list Args)
{
    CURLcode Req = oCUrl_easy_setopt(Handle, Option, Args);

    // this option is always the first being called
    if (Option == CURLOPT_URL)
    {
        const char* URL = va_arg(Args, const char*);
        std::cerr << "\n\n[CURLOPT_URL] " << URL << "\n";
        output << "\n\n[CURLOPT_URL] " << URL << "\n";
    }
    else if (Option == CURLOPT_HTTPGET)
    {
        std::cerr << "[CURL METHOD] GET\n";
        output << "[CURL METHOD] GET\n";
    }
    else if (Option == CURLOPT_POST)
    {
        std::cerr << "[CURL METHOD] POST\n";
        output << "[CURL METHOD] POST\n";
    }
    else if (Option == CURLOPT_UPLOAD)
    {
        std::cerr << "[CURL METHOD] PUT\n";
        output << "[CURL METHOD] PUT\n";
    }
    else if (Option == CURLOPT_CUSTOMREQUEST)
    {
        const char* Method = va_arg(Args, const char*);
        std::cerr << "[CURL CUSTOM METHOD] " << Method << "\n";
        output << "[CURL CUSTOM METHOD] " << Method << "\n";
    }
    else if (Option == CURLOPT_NOBODY)
    {
        std::cerr << "[CURL METHOD] HEAD\n";
        output << "[CURL METHOD] HEAD\n";
    }
    else if (Option == CURLOPT_HTTPHEADER)
    {
        curl_slist* Headers = va_arg(Args, curl_slist*);
        curl_slist* Current = Headers;

        std::vector<std::string> HeadersList;
        while (Current)
        {
            HeadersList.push_back(Current->data);
            Current = Current->next;
        }

        for (auto& Header : HeadersList)
        {
            std::cerr << "[HEADER] " << Header << "\n";
            output << "[HEADER] " << Header << "\n";
        }
    }
    else if (Option == CURLOPT_USERAGENT)
    {
        const char* UserAgent = va_arg(Args, const char*);
        std::cerr << "[USER-AGENT] " << UserAgent << "\n";
        output << "[USER-AGENT] " << UserAgent << "\n";
    }
    else if (Option == CURLOPT_READDATA)
    {
        FCurlHttpRequest* Instance = va_arg(Args, FCurlHttpRequest*);
        if (!Instance)
        {
            std::cerr << "[CURLOPT_READDATA] FCurlHttpRequest* Instance was nullptr!\n";
            return Req;
        }

        FRequestPayloadInMemory* Payload = (FRequestPayloadInMemory*)Instance->RequestPayload;
        if (!Payload)
        {
            std::cerr << "[CURLOPT_READDATA] Payload is nullptr!\n";
            return Req;
        }

        TArray<uint8_t> Buffer = Payload->Buffer;
        if (Buffer.Num() < 1)
        {
            std::cerr << "[CURLOPT_READDATA] Buffer Data is invalid!\n";
            return Req;
        }

        const char* ConvertedData = reinterpret_cast<const char*>(Buffer.GetData());
        std::string PayloadStr(ConvertedData, Buffer.Num());

        std::cerr << "[PAYLOAD BUFFER] " << PayloadStr << "\n";
        output << "[PAYLOAD BUFFER] " << PayloadStr << "\n";
    }

    return Req;
}
