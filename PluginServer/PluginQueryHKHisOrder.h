#pragma once
#include "Include/FTPluginCore.h"
#include "Include/FTPluginQuoteInterface.h"
#include "Include/FTPluginTradeInterface.h"
#include "Protocol/ProtoDataStruct_Trade.h"
#include "TimerWnd.h"
#include "MsgHandler.h"
#include "JsonCpp/json.h"

class CPluginHKTradeServer;

class CPluginQueryHKHisOrder : public CTimerWndInterface, public CMsgHandlerEventInterface
{
public:
	CPluginQueryHKHisOrder();
	virtual ~CPluginQueryHKHisOrder();
	
	void Init(CPluginHKTradeServer* pTradeServer, ITrade_HK*  pTradeOp);
	void Uninit();	
	void SetTradeReqData(int nCmdID, const Json::Value &jsnVal, SOCKET sock);
	void NotifyOnQueryHKHisOrder(Trade_Env enEnv, UINT32 nCookie, INT32 nCount, const Trade_OrderItem* pArrOrder);

	void NotifySocketClosed(SOCKET sock);

protected:
	//CTimerWndInterface 
	virtual void OnTimeEvent(UINT nEventID);

	//CMsgHandlerEventInterface
	virtual void OnMsgEvent(int nEvent,WPARAM wParam,LPARAM lParam);

protected:
	//tomodify 1
	typedef QueryHKHisOrder_Req	TradeReqType;
	typedef QueryHKHisOrder_Ack	TradeAckType;
	typedef QueryHKHisOrderAckItem TradeAckItemType;

	struct	StockDataReq
	{
		SOCKET	sock;
		DWORD	dwReqTick;
		DWORD	dwLocalCookie;
		TradeReqType req;
	};
	
	typedef std::vector<StockDataReq*>		VT_REQ_TRADE_DATA;	
	
protected:	
	void HandleTimeoutReq();
	void HandleTradeAck(TradeAckType *pAck, SOCKET	sock);
	void SetTimerHandleTimeout(bool bStartOrStop);
	void ClearAllReqAckData();
	
private: 
	bool DoDeleteReqData(StockDataReq* pReq); 
	void DoGetFilterStatus(const std::string& strFilter, std::set<int>& setStatus);
	void DoGetFilterCode(const std::string& strFilter, std::set<std::wstring>& setCode);
	bool IsFitFilter(const TradeAckItemType& AckItem,
		const std::set<int>& setStatus, const std::set<std::wstring>& setCode);

private:
	void DoClearReqInfo(SOCKET socket);

protected:
	CPluginHKTradeServer	*m_pTradeServer;
	ITrade_HK				*m_pTradeOp;	
	BOOL					m_bStartTimerHandleTimeout;
	
	CTimerMsgWndEx		m_TimerWnd;
	CMsgHandler			m_MsgHandler;

	VT_REQ_TRADE_DATA	m_vtReqData;
};