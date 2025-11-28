////===========================================================================================================================================================
//// 
//// カード情報 [card_info.cpp]
//// Author : Kasai Keisuke
//// 
////===========================================================================================================================================================
//
//// include 
//#include "card_info.h"
//
///**
//* @brief コンストラクタ
//*/
//My::CCardInfo::CCardInfo()
//{
//	m_Param = {};
//	m_Record = {};
//}
//
///**
//* @brief デストラクタ
//*/
//My::CCardInfo::~CCardInfo()
//{
//	m_Param = {};
//	m_Record = {};
//}
//
///**
//* @brief 初期化処理
//* @return 成功したかどうか
//*/
//HRESULT My::CCardInfo::Init()
//{
//	return S_OK;
//}
//
///**
//* @brief 読み込み処理
//*/
//HRESULT My::CCardInfo::LoadFile(const std::string& path)
//{
//	std::unique_lock<std::shared_mutex> lock(m_mutex);
//
//	std::vector<uint8_t>buf;
//
//	HRESULT hr = readFileToBuffer(path, buf);
//
//    if (FAILED(hr))
//    {
//        return hr;
//    }
//
//    bool isCbor = isCborBuffer(buf);
//
//    nlohmann::ordered_json root;
//
//    try
//    {
//        if (isCbor)
//        {
//            root = nlohmann::ordered_json::from_cbor(buf);
//        }
//        else 
//        {
//            std::string text(buf.begin(), buf.end());
//            root = nlohmann::ordered_json::parse(text);
//        }
//    }
//    catch (const std::exception& e) 
//    {
//        std::cerr << "パース失敗" << std::endl;
//
//        return E_FAIL;
//    }
//
//    nlohmann::ordered_json arr;
//    if (root.is_array())
//    {
//        arr = root;
//    }
//    else if (root.is_object() && root.contains("cards"))
//    {
//        arr = root["cards"];
//    }
//    else
//    {
//        return E_FAIL;
//    }
//
//    for (const auto& item : arr)
//    {
//        try
//        {
//            CardRecord rec = FromJsonToRecord(item);
//        }
//    }
//
//	return S_OK;
//}
//
//HRESULT My::CCardInfo::readFileToBuffer(const std::string& path, std::vector<uint8_t>& outBuf) const
//{
//    if (path.empty())
//    {
//        return E_INVALIDARG;
//    }
//
//    try 
//    {
//        std::ifstream ifs(path, std::ios::binary);
//        if (!ifs) 
//        {
//            // ファイルが開けない（存在しない・権限不足など）
//            return E_FAIL;
//        }
//
//        // ファイルサイズを取得して予約（可能な場合）
//        std::streampos begin = ifs.tellg();
//        ifs.seekg(0, std::ios::end);
//        std::streampos end = ifs.tellg();
//        ifs.seekg(0, std::ios::beg);
//
//        outBuf.clear();
//        if (end > begin) 
//        {
//            size_t fileSize = static_cast<size_t>(end - begin);
//            outBuf.reserve(fileSize);
//        }
//
//        constexpr std::size_t CHUNK = 4096;
//        std::vector<char> tmp(CHUNK);
//
//        while (ifs) 
//        {
//            ifs.read(tmp.data(), static_cast<std::streamsize>(tmp.size()));
//            std::streamsize n = ifs.gcount();
//            if (n > 0) 
//            {
//                outBuf.insert(outBuf.end(), reinterpret_cast<uint8_t*>(tmp.data()),
//                    reinterpret_cast<uint8_t*>(tmp.data() + n));
//            }
//        }
//
//        if (ifs.bad()) 
//        {
//            // ストリームエラー（読み取り中の致命的エラー）
//            outBuf.clear();
//            return E_FAIL;
//        }
//
//        return S_OK;
//    }
//    catch (const std::bad_alloc&) 
//    {
//        // メモリ不足
//        outBuf.clear();
//        return E_OUTOFMEMORY;
//    }
//    catch (const std::exception&)
//    {
//        // その他の例外
//        outBuf.clear();
//        return E_FAIL;
//    }
//}
//
//bool My::CCardInfo::isCborBuffer(const std::vector<uint8_t>& buf) const
//{
//    if (buf.empty())
//    {
//        return false;
//    }
//
//    // 1) self-describe CBOR (RFC 8742): 0xD9 0xD9 0xF7
//    if (buf.size() >= 3 && buf[0] == 0xD9 && buf[1] == 0xD9 && buf[2] == 0xF7) 
//    {
//        return true;
//    }
//
//    // 2) 先頭の非空白バイトを探す（テキストファイルの可能性を判定）
//    size_t i = 0;
//    while (i < buf.size()) 
//    {
//        uint8_t b = buf[i];
//        if (b == ' ' || b == '\t' || b == '\r' || b == '\n')
//        { 
//            ++i; 
//            continue; 
//        }
//        // テキスト JSON の開始文字
//        if (b == '{' || b == '[')
//        {
//            return false;
//        }
//        // ASCII テキストの可能性が高い場合は JSON と判断する（例: '"' や数字もテキスト）
//        if ((b >= 0x20 && b <= 0x7E))
//        {
//            // ただし ASCII でも CBOR の先頭バイトと被る可能性があるため次の判定へ
//            break;
//        }
//        break;
//    }
//    if (i >= buf.size())
//    {
//        return false; // 空白のみ -> treat as JSON/text
//    }
//
//    // 3) CBOR の major type をチェック（先頭バイト）
//    uint8_t first = buf[i];
//    uint8_t major = (first >> 5) & 0x07; // 上位3ビット
//    // CBOR の major type は 0..7（常にこの範囲）だが、テキスト開始文字と被る場合を除外する
//    // ここでは heuristic: 先頭バイトが ASCII printable の場合は JSON の可能性を優先する
//    if (!(first == '{' || first == '[' || first == '"' || (first >= '0' && first <= '9'))) 
//    {
//        // 典型的な CBOR の先頭バイトは 0x00..0xFF の任意値だが、
//        // ASCII printable と明確に被らない場合は CBOR と判断して良い
//        return true;
//    }
//
//    // 4) フォールバック: JSON とみなす
//    return false;
//}
