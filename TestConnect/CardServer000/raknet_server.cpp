//================================
//
// TCP接続のサーバー用のcpp[raknet_server.cpp]
// Author:Yuuto Shimadu
//
//================================

//ヘッダーのインクルード
#include "raknet_server.h"

//静的変数の宣言
CRakNet_Data* CRakNet_Server::m_pRakNetData = nullptr;

//=====================================
//コンストラクタ
//=====================================
CRakNet_Server::CRakNet_Server() :
	m_pPacket(nullptr),	//パケット
    m_CardVector()      //カードのベクター
{
    //動的確保
    //m_pRakNetData = new CLobby_Data;
}

//=====================================
//デストラクタ
//=====================================
CRakNet_Server::~CRakNet_Server()
{
    //メモリ開放
    if (m_pRakNetData != nullptr)
    {
        delete m_pRakNetData;
        m_pRakNetData = nullptr;
    }
}

//=====================================
//初期化処理
//=====================================
bool CRakNet_Server::Init(int nPortNum, RakNet::RakPeerInterface* peer)
{
	//ピアの取得
	RakNet::SocketDescriptor sd(nPortNum, 0);
    peer->Startup(MAX_CLIENTS, &sd, 1);
	std::cout << "Starting the server.\n";
    peer->SetMaximumIncomingConnections(MAX_CLIENTS);

    //LAN検出用にアドバタイズ（ブロードキャスト）
    peer->AdvertiseSystem("255.255.255.255", nPortNum, 0, 0, 0);

    //切断を確認する時間を5秒に変更
    peer->SetTimeoutTime(5000, RakNet::UNASSIGNED_SYSTEM_ADDRESS);

	return true;
}

//=====================================
//接続受付の処理
//=====================================
void CRakNet_Server::Accept(void)
{

}

//=====================================
//終了処理
//=====================================
void CRakNet_Server::Uninit(RakNet::RakPeerInterface* peer)
{
    RakNet::RakPeerInterface::DestroyInstance(peer);
}

//=====================================
//通信処理
//=====================================
void CRakNet_Server::Communication(RakNet::RakPeerInterface* peer)
{
    //変数宣言
    RakNet::Packet* packet;

    //常時周回
    while (1) 
    {
        //読み込み処理
        for (packet = peer->Receive(); packet; peer->DeallocatePacket(packet), packet = peer->Receive())
        {
            switch (packet->data[0]) 
            {
            case ID_REMOTE_DISCONNECTION_NOTIFICATION:
                std::cout << "別のクライアントが切断されました\n";
                break;
            case ID_REMOTE_CONNECTION_LOST:
                std::cout << "別のクライアントが接続を失いました.\n";
                break;
            case ID_REMOTE_NEW_INCOMING_CONNECTION:
                std::cout << "別のクライアントが接続しました\n";
                break;
            case ID_CONNECTION_REQUEST_ACCEPTED:
                std::cout << "接続リクエストが承認されました\n";
                break;
            case ID_NEW_INCOMING_CONNECTION:
                std::cout << "新しいクライアントが接続しました\n";
                m_pRakNetData->NewConnection(packet, peer);
                break;
            case ID_NO_FREE_INCOMING_CONNECTIONS:
                std::cout << "サーバーがいっぱいです\n";
                break;
            case ID_DISCONNECTION_NOTIFICATION:
                std::cout << "クライアントが切断されました\n";
                m_pRakNetData->DisConnection(packet, peer);
                break;
            case ID_CONNECTION_LOST:
                std::cout << "クライアントの接続が失われました\n";
                m_pRakNetData->DisConnection(packet, peer);
                break;
            case CRakNet_Data::ID_CARD_MESSAGE_1:
                std::cout << "指定番号のカードの情報";
                m_pRakNetData->DisConnection(packet, peer);
                break;
           
            default:
                std::cout << "Message with identifier " << (int)packet->data[0] << " has arrived.\n";
                break;
            }
        }
    }
}

//=====================================
//特定のクライアントとの通信処理
//=====================================
void CRakNet_Server::Communication0(RakNet::RakPeerInterface* peer, RakNet::Packet* packet)
{

}

//=====================================
//データの変更
//=====================================
void CRakNet_Server::ChangeData(CRakNet_Data* data)
{
  
}

//=====================================
//カードの読み込み
//=====================================
void CRakNet_Server::LoadCard()
{
    // コンソールの出力コードページを UTF-8 に変更
    SetConsoleOutputCP(CP_UTF8);

    //std::locale::global(std::locale(""));

    //ファイルを開く
    std::ifstream ifs("json\\cards.json");

    //開けたかを確認
    if (!ifs.is_open())
    {
        assert(false && "ファイルを開けませんでした");
        return;
    }

    std::string fileData((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
    ifs.close();

    // BOM除去（UTF-8の先頭3バイト）
    if (fileData.size() >= 3 &&
        static_cast<unsigned char>(fileData[0]) == 0xEF &&
        static_cast<unsigned char>(fileData[1]) == 0xBB &&
        static_cast<unsigned char>(fileData[2]) == 0xBF)
    {
        fileData.erase(0, 3);
    }

    // JSONパース（日本語含む）
    ordered_json Json/* = ordered_json::parse(fileData)*/;

    try
    {
        // 文字列から直接パース(日本語変換)
        Json = ordered_json::parse(fileData);
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
                        //std::cout << ", Value: " << val.get<int>() << std::endl;
                    }
                    else if (key == "Card ID")
                    {

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
   
    //バイナリファイルを書き出し
    std::ofstream ofs("File\\test.bin", std::ios_base::binary);
    ofs.write((char*)&m_CardVector[0], sizeof(CCard));
    ofs.close();

    //読み込み
    CCard Test = {};
    std::ifstream b("File\\test.bin", std::ios::binary);
    b.read((char*)&Test, sizeof(CCard));
    b.close();
}
