// Fill out your copyright notice in the Description page of Project Settings.


#include "LoginPacketManager.h"

PacketDispatcher::PacketDispatcher(TQueue<uint8*>* Queue) : PacketQueue(Queue)
{
}


