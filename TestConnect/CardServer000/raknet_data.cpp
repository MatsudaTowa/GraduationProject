//================================
//
// 送受信データクラス用のヘッダー[taknet_data.cpp]
// Author:Yuuto Shimadu
//
//================================

//ヘッダーのインクルード
#include "raknet_data.h"
#include "tiny_aes/aes.h"

/**
* @brief CBOR暗号化
*/
std::vector<uint8_t> AES_Encrypt(const std::vector<uint8_t>& plaintext,
    const std::vector<uint8_t>& key,
    const std::vector<uint8_t>& iv)
{
    std::vector<uint8_t> buffer = plaintext;

    // PKCS7風パディング（1〜16バイト）
    size_t pad_len = AES_BLOCKLEN - (buffer.size() % AES_BLOCKLEN);
    buffer.insert(buffer.end(), pad_len, static_cast<uint8_t>(pad_len));

    AES_ctx ctx;
    AES_init_ctx_iv(&ctx, key.data(), iv.data());
    AES_CBC_encrypt_buffer(&ctx, buffer.data(), buffer.size());

    return buffer;
}

/**
* @brief CBOR複合化
*/
std::vector<uint8_t> AES_Decrypt(const std::vector<uint8_t>& ciphertext,
    const std::vector<uint8_t>& key,
    const std::vector<uint8_t>& iv)
{
    std::vector<uint8_t> buffer = ciphertext;

    AES_ctx ctx;
    AES_init_ctx_iv(&ctx, key.data(), iv.data());
    AES_CBC_decrypt_buffer(&ctx, buffer.data(), buffer.size());

    // PKCS7風パディング除去
    if (!buffer.empty()) {
        uint8_t pad_len = buffer.back();
        if (pad_len > 0 && pad_len <= AES_BLOCKLEN && buffer.size() >= pad_len) {
            buffer.resize(buffer.size() - pad_len);
        }
    }

    return buffer;
}

//=====================================
//コンストラクタ
//=====================================
CRakNet_Data::CRakNet_Data() :
    m_CardVector()  //カードリストのクリア
{
    m_CardVector.clear();
}

//=====================================
//新しいクライアントの接続処理
//=====================================
void CRakNet_Data::NewConnection(RakNet::Packet* packet, RakNet::RakPeerInterface* peer)
{
    
}

//=====================================
//クライアントの切断処理
//=====================================
void CRakNet_Data::DisConnection(RakNet::Packet* packet, RakNet::RakPeerInterface* peer)
{
    
}

//=====================================
//カードの読み込み
//=====================================
void CRakNet_Data::LoadCard()
{
    // コンソールの出力コードページを UTF-8 に変更
    SetConsoleOutputCP(CP_UTF8);

    //std::locale::global(std::locale(""));

    //ファイルを開く
    std::ifstream ifs("json\\cards.cbor", std::ios::binary);

    //開けたかを確認
    if (!ifs.is_open())
    {
        assert(false && "ファイルを開けませんでした");
        return;
    }

    std::vector<uint8_t> encrypted((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
    //std::string fileData((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
    ifs.close();

    // 鍵とIV（Save関数と同じ値を使う）
    std::vector<uint8_t> key(32, 0x01);
    std::vector<uint8_t> iv(16, 0x02);

    // AES-256-CBC 復号
    std::vector<uint8_t> decrypted = AES_Decrypt(encrypted, key, iv);

    // BOM除去（UTF-8の先頭3バイト）
    /*if (fileData.size() >= 3 &&
        static_cast<unsigned char>(fileData[0]) == 0xEF &&
        static_cast<unsigned char>(fileData[1]) == 0xBB &&
        static_cast<unsigned char>(fileData[2]) == 0xBF)
    {
        fileData.erase(0, 3);
    }*/

    // JSONパース（日本語含む）
    ordered_json Json/* = ordered_json::parse(fileData)*/;

    try
    {
        // 文字列から直接パース(日本語変換)
        //Json = ordered_json::parse(fileData);
        // CBOR → JSON に変換
        Json = ordered_json::from_cbor(decrypted);
    }
    catch (const std::exception& e)
    {
        // MessageBox(m_hWnd, L"JSON のパースに失敗しました", L"エラー", MB_OK);
        std::cout << "JSON parse error: " << e.what() << "\n";
        return;
    }

    //文字列から直接パース
    //ordered_json Json;
    //ifs >> Json;

    // ルート情報とキー一覧を出力（デバッグ）
    std::cout << "Root type: " << Json.type_name() << "\n";
    std::cout << "Root dump:\n" << Json.dump(2) << "\n";

    std::cout << "--- Parsed JSON Content ---" << std::endl;
    std::cout << Json.dump(4) << std::endl; // dump(4)で見やすく整形して出力
    std::cout << "---------------------------" << std::endl;

    if (Json.contains(u8"カード") && Json[u8"カード"].is_array())
    {
        const ordered_json& cards_array = Json[u8"カード"];

        for (const auto& card : cards_array)
        {
            CCard Card;
            if (card.is_object())
            {
                std::cout << "--------------------------------" << std::endl;

                // 5. 【ここを変更】従来のイテレーターループを使用する
                for (ordered_json::const_iterator it = card.begin(); it != card.end(); ++it)
                {
                    // key() メンバ関数でキーを取得
                    const std::string& key = it.key();
                    // value() メンバ関数で値を取得
                    const ordered_json& val = it.value();

                    //パラメータの代入
                    if (key == "Pack ID")
                    {//パック番号
                        Card.GetParam().nPackID = val.get<int>();
                    }
                    else if (key == "Card ID")
                    {
                        Card.GetParam().nCardID = val.get<int>();
                    }
                    else if (key == "Card Name")
                    {
                        Card.GetParam().Name = val.get<std::string>();
                    }
                    else if (key == "Card Name Ruby")
                    {
                        Card.GetParam().Ruby = val.get<std::string>();
                    }
                    else if (key == "image")
                    {
                        Card.GetParam().ImagePath = val.get<std::string>();
                    }
                    else if (key == "cost")
                    {
                        Card.GetParam().nCost = val.get<int>();
                    }
                    else if (key == "rarity")
                    {
                        Card.GetParam().Raritytype = static_cast<CCard::RARITY>(val.get<int>());
                    }
                    else if (key == "type")
                    {
                        Card.GetParam().Maintype = static_cast<CCard::CardType>(val.get<int>());
                    }
                    else if (key == "attacktype")
                    {
                        Card.GetParam().Attacktype = static_cast<CCard::AttackType>(val.get<int>());
                    }
                    else if (key == "power")
                    {
                        Card.GetParam().nDamage = val.get<int>();
                    }
                    else if (key == "defensetype")
                    {
                        Card.GetParam().Defensetype = static_cast<CCard::DefenseType>(val.get<int>());
                    }
                    else if (key == "guard")
                    {
                        Card.GetParam().nGuard = val.get<int>();
                    }
                    else if (key == "counter")
                    {
                        Card.GetParam().nGuard = val.get<int>();
                    }
                    else if (key == "assisttype")
                    {
                        Card.GetParam().Assisttype = static_cast<CCard::AssistType>(val.get<int>());
                    }
                    else if (key == "bufftype")
                    {
                        Card.GetParam().Bufftype = static_cast<CCard::BuffType>(val.get<int>());
                    }
                    else if (key == "healtype")
                    {
                        Card.GetParam().Healtype = static_cast<CCard::HealType>(val.get<int>());
                    }
                    else if (key == "heal")
                    {
                        Card.GetParam().nHeal = val.get<int>();
                    }

                    std::cout << "  Key: " << key;

                    if (val.is_string())
                    {
                        std::cout << ", Value: " << val.get<std::string>() << std::endl;
                    }
                    else if (val.is_number_integer())
                    {
                        std::cout << ", Value: " << val.get<int>() << std::endl;
                    }
                    else
                    {
                        std::cout << ", Value: " << val << std::endl;
                    }
                }
            }

            m_CardVector.push_back(Card);
        }
    }
}

//=====================================
//カードの送信処理
//=====================================
void CRakNet_Data::SendCard(RakNet::Packet* packet, RakNet::RakPeerInterface* peer)
{
    //データの受信
    RakNet::BitStream bsIn(packet->data, packet->length, false);

    //読み取り用変数
    int nCardID = 0;

    //読み取り
    bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
    bsIn.Read(nCardID);

    //カードIDより小さい値なら飛ばす
    if (nCardID < 0)
    {
        return;
    }

    //データの作成
    RakNet::BitStream bsOut;
    bsOut.Write((RakNet::MessageID)GameMessages::ID_CARD_MESSAGE_1);
    bsOut.Write(m_CardVector[nCardID - 1].GetParam());

    //受信したクライアントにブロードキャスト
    peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->guid, false);
}

//=====================================
//全てのカードの送信処理
//=====================================
void CRakNet_Data::SendAllCard(RakNet::Packet* packet, RakNet::RakPeerInterface* peer)
{
    //データの受信
    RakNet::BitStream bsIn(packet->data, packet->length, false);

    //読み取り
    bsIn.IgnoreBytes(sizeof(RakNet::MessageID));

    //データの作成
    RakNet::BitStream bsOut;
    bsOut.Write((RakNet::MessageID)GameMessages::ID_ALLCARD_MESSAGE_1); //メッセージ
    bsOut.Write((int)m_CardVector.size());                              //カード総数

    //全カードの情報を送信
    for (auto iter : m_CardVector)
    {
        //bsOut.Write(iter.GetParam());
        RakNet::RakString msg;

        //書き出し
        bsOut.Write(iter.GetParam().nPackID);
        bsOut.Write(iter.GetParam().nCardID);
        msg = iter.GetParam().Name.c_str();
        bsOut.Write(msg); msg.Clear();
        msg = iter.GetParam().Ruby.c_str();
        bsOut.Write(msg); msg.Clear();
        bsOut.Write(iter.GetParam().nCost);
        bsOut.Write(iter.GetParam().nDamage);
        bsOut.Write(iter.GetParam().nGuard);
        bsOut.Write(iter.GetParam().nCounter);
        bsOut.Write(iter.GetParam().nHeal);
        bsOut.Write(static_cast<int>(iter.GetParam().Maintype));
        bsOut.Write(static_cast<int>(iter.GetParam().Raritytype));
        bsOut.Write(static_cast<int>(iter.GetParam().Assisttype));
        bsOut.Write(static_cast<int>(iter.GetParam().Attacktype));
        bsOut.Write(static_cast<int>(iter.GetParam().Defensetype));
        bsOut.Write(static_cast<int>(iter.GetParam().Bufftype));
        bsOut.Write(static_cast<int>(iter.GetParam().Healtype));
        msg = iter.GetParam().ImagePath.c_str();
        bsOut.Write(msg);
    }

    //受信したクライアントにブロードキャスト
    peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->guid, false);
}
