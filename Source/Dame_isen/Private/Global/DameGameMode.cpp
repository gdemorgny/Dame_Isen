// Fill out your copyright notice in the Description page of Project Settings.

#include "Global/DameGameMode.h"
#include "GamePlay/DameBoard.h"
#include "GamePlay/DamePion.h"
#include "GamePlay/DameCase.h"



void ADameGameMode::InitializeBoard()
{
	FRotator myRot(0, 0, 0);
	FVector myLoc(0, 0, 0);
	FActorSpawnParameters parameters;
	for(int i = 0; i < BoardSize; i++)
	{
		for(int j = 0; j < BoardSize; j++)
		{
			myLoc.X = j*100;
			myLoc.Y = i*100;
			
			DameGameState.Cells.Add(GetWorld()->SpawnActor<ADameCase>(CaseActor, myLoc, myRot, parameters));
			DameGameState.Cells.Last()->CellId = i*8+j;
			DameGameState.Cells.Last()->CellColorType = (i%2 ==0 && j%2 == 1 ) || (i%2 == 1 && j%2 == 0);
			DameGameState.Cells.Last()->ChangeMaterial(false);
		}
	}
	//Board->IntializeBoard();
}

void ADameGameMode::InitializePlayers()
{
	DameGameState.PlayerPawns.SetNum(2);
	int32 CellNumber;
	//TODO : Faire apparaitre les pions au bons endroits !!!
	for (int i = 0; i < 24; i++) {
		
		if (i < 12)
		{
			CellNumber = i* 2;
			
		} else 
		{
			CellNumber=i*2+16;
		}
		CellNumber = ((i/4)%2 == 1)?CellNumber+1:CellNumber;
		DameGameState.PlayerPawns[0].Pawns.Add(SpawnPion(CellNumber,CellNumber < 24));
		DameGameState.PlayerPawns[0].Pawns.Last()->PawnId = i;
		DameGameState.Links.Add(FLinksPlayerCell(i,CellNumber));
	}
}

void ADameGameMode::ChangePlayer()
{
	DameGameState.ActualPlayer = -DameGameState.ActualPlayer;
	CheckMandatoryCapture();
}

void ADameGameMode::CheckMandatoryCapture()
{
	UE_LOG(LogTemp,Warning,TEXT("Check possible move for player %d"),DameGameState.ActualPlayer);
	CaptureMandatoryNumber = 0;
	uint8 PlayerNumber = (DameGameState.ActualPlayer == 1)?0:1;
	uint8 OtherPlayerNumber = (-DameGameState.ActualPlayer == 1)?0:1;
	TArray<int32> PawnsIndex;
	for (int i = 0 ; i<DameGameState.PlayerPawns[OtherPlayerNumber].Pawns.Num();i++)
	{
		DameGameState.PlayerPawns[OtherPlayerNumber].Pawns[i]->ChangeMaterial(false);
	}
	for (int i = 0 ; i<DameGameState.PlayerPawns[PlayerNumber].Pawns.Num();i++)
	{
		TArray<FMovementSequence> CaptureMoves = Board->CheckMoves(GetCellIdByPawnId(DameGameState.PlayerPawns[PlayerNumber].Pawns[i]->PawnId), DameGameState.ActualPlayer,false);
		if(CaptureMoves.Num() > 0)
		{
			for (const FMovementSequence& Move : CaptureMoves)
			{
				DameGameState.PlayerPawns[PlayerNumber].Pawns[i]->CaptureMaxAllowed = Move.Cells.Num() / 2;
				
				if (Move.Cells.Num() > 1 && Move.Cells.Num() / 2 > CaptureMandatoryNumber)
				{
					CaptureMandatoryNumber = Move.Cells.Num()/2;
					// Capture maximale trouvée
					UE_LOG(LogTemp,Warning,TEXT("Find Capture for Pawn %d"),i);
				}
			}
		} else
		{
			DameGameState.PlayerPawns[PlayerNumber].Pawns[i]->CaptureMaxAllowed = 0;
		}
		
	}
	UE_LOG(LogTemp,Warning,TEXT("Capture number is : %d"), CaptureMandatoryNumber);
	for (int i = 0 ; i<DameGameState.PlayerPawns[PlayerNumber].Pawns.Num();i++)
	{
		if(DameGameState.PlayerPawns[PlayerNumber].Pawns[i]->CaptureMaxAllowed>0 && DameGameState.PlayerPawns[PlayerNumber].Pawns[i]->CaptureMaxAllowed == CaptureMandatoryNumber)
		{
			DameGameState.PlayerPawns[PlayerNumber].Pawns[i]->ChangeMaterial(true);
		}
	}
	
}

ADamePion* ADameGameMode::SpawnPion(int32 CellNumber, bool PawnColorType)
{
	FActorSpawnParameters parameters;
	FRotator myRot(0, 0, 0);
	ADamePion* tempPion = GetWorld()->SpawnActor<ADamePion>(PionActor, DameGameState.Cells[CellNumber]->GetActorLocation(), myRot, parameters);
	tempPion->PawnColorType = PawnColorType;
	tempPion->Board = Board;
	tempPion->DameGM = this;
	tempPion->ChangeMaterial(false);
	return tempPion;
}
bool ADameGameMode::IsCellEmpty(int32 CellId)
{
	return GetPawnIdByCellNumber(CellId) == -1;
}
int32 ADameGameMode::GetActualPlayer() const
{
	return DameGameState.ActualPlayer;
}

bool ADameGameMode::IsPawnControlledByActualPlayer(int32 PawnId)
{
	return GetPlayerIdByPawnId(PawnId) == DameGameState.ActualPlayer;
}

int32 ADameGameMode::GetPlayerIdByPawnId(int32 PawnId)
{
	int32 result = (PawnId <12)?0:1;
	return result;
}

int32 ADameGameMode::GetCellIdByPawnId(int32 PawnId)
{
	int32 result = -1;
	for(int i = 0;i<DameGameState.Links.Num();i++)
	{
		if(DameGameState.Links[i].PawnId == PawnId)
		{
			result = DameGameState.Links[i].CellId;
			break;
		}
	}
	return result;
}

ADameCase* ADameGameMode::GetCellByPawnId(int32 PawnId)
{
	ADameCase* result = DameGameState.Cells[GetCellIdByPawnId(PawnId)];
	
	return result;
}

ADameCase* ADameGameMode::GetCellByCellId(int32 CellId)
{
	return DameGameState.Cells[CellId];
}

int32 ADameGameMode::GetPawnIdByCellNumber(int32 CellId)
{
	int32 result = -1;
	for(int i = 0;i<DameGameState.Links.Num();i++)
	{
		if(DameGameState.Links[i].CellId == CellId)
		{
			result = DameGameState.Links[i].PawnId;
			break;
		}
	}
	return result;
}

ADamePion* ADameGameMode::GetPawnByCellNumber(int32 CellId)
{
	int32 PawnId = GetPawnIdByCellNumber(CellId);
	ADamePion* result = nullptr;
	for(int i = 0;i<DameGameState.PlayerPawns[(PawnId<12)?0:1].Pawns.Num();i++)
	{
		if (DameGameState.PlayerPawns[(PawnId<12)?0:1].Pawns[i]->PawnId == PawnId)
		{
			result = DameGameState.PlayerPawns[(PawnId<12)?0:1].Pawns[i];
			break;
		}
	}
	
	return result;
}

int32 ADameGameMode::GetPawnNumberByPlayerId(int32 PlayerId)
{
	return DameGameState.PlayerPawns[PlayerId].Pawns.Num();
}

TArray<ADamePion*> ADameGameMode::GetPawnsByPlayerId(int32 PlayerId)
{
	return DameGameState.PlayerPawns[PlayerId].Pawns;
}

void ADameGameMode::RemovePawnAtIndexByPlayerId(int32 PlayerId, int32 index)
{
	RemoveLinkByPawnId(DameGameState.PlayerPawns[PlayerId].Pawns[index]->PawnId);
	DameGameState.PlayerPawns[PlayerId].Pawns.RemoveAt(index);
}

void ADameGameMode::RemovePawnByPawnId(int32 PawnId)
{
	for(int i=0;i<DameGameState.PlayerPawns.Num();i++)
	{
		for(int j=0;j<GetPawnNumberByPlayerId(i);j++)
		{
			if(DameGameState.PlayerPawns[i].Pawns[j]->PawnId == PawnId)
			{
				RemoveLinkByPawnId(PawnId);
				DameGameState.PlayerPawns[i].Pawns.RemoveAt(j);
				return;
			}
		}
	}
}

void ADameGameMode::RemoveLinkByPawnId(int32 PawnId)
{
	for(int i = 0;i<DameGameState.Links.Num();i++)
	{
		if(DameGameState.Links[i].PawnId == PawnId)
		{
			DameGameState.Links.RemoveAt(i);
			return;
		}
	}
}

void ADameGameMode::RemoveLinkByCellId(int32 CellId)
{
	for(int i = 0;i<DameGameState.Links.Num();i++)
	{
		if(DameGameState.Links[i].CellId == CellId)
		{
			DameGameState.Links.RemoveAt(i);
			return;
		}
	}
}

void ADameGameMode::SetNewLink(int32 PawnId, int32 CellId)
{
	RemoveLinkByPawnId(PawnId);
	DameGameState.Links.Add(FLinksPlayerCell(PawnId,CellId));
}
