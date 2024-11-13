// Fill out your copyright notice in the Description page of Project Settings.

#include "Global/DameGameMode.h"
#include "GamePlay/DameBoard.h"
#include "GamePlay/DamePion.h"
#include "GamePlay/DameCase.h"



void ADameGameMode::InitializeBoard()
{
	Board->IntializeBoard();
}

void ADameGameMode::InitializePlayers()
{
	PlayerPawns.SetNum(2);
	//TODO : pr�parer le tableau de tableau avec les valeur 1 et -1 dnas les bonnes cases 
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (i < 3 && (j + i%2) % 2 == 0) {
				//player 0
				Board->RowCell[i].Column[j] = 1;
			}
			else if (i > 4 && (j + i % 2) % 2 == 0) {
				//player1
				Board->RowCell[i].Column[j] = -1;
	
			}
		}
	}

	//TODO : Faire apparaitre les pions au bons endroits !!!
	int CellNumber;
	int Playeid;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			CellNumber = i * 8 + j;
			if (i < 3 && (j + i % 2) % 2 == 0) {
				Playeid = 1;
				SpawnPion(CellNumber,Playeid);
			}
			else if (i > 4 && (j + i % 2) % 2 == 0) {
				Playeid = -1;
				SpawnPion(CellNumber,Playeid);
			}
			
			
		}
	}
	
	Board->DameGameState = Board->GetCurrentGameState();

}

void ADameGameMode::ChangePlayer()
{
	ActualPlayer = -ActualPlayer;
	Board->DameGameState = Board->GetCurrentGameState();
	CheckMandatoryCapture();
}

void ADameGameMode::CheckMandatoryCapture()
{
	UE_LOG(LogTemp,Warning,TEXT("Check possible move for player %d"),ActualPlayer);
	CaptureMandatoryNumber = 0;
	uint8 PlayerNumber = (ActualPlayer == 1)?0:1;
	uint8 OtherPlayerNumber = (-ActualPlayer == 1)?0:1;
	TArray<int32> PawnsIndex;
	for (int i = 0 ; i<PlayerPawns[OtherPlayerNumber].Pawns.Num();i++)
	{
		PlayerPawns[OtherPlayerNumber].Pawns[i]->ChangeMaterial(false);
	}
	for (int i = 0 ; i<PlayerPawns[PlayerNumber].Pawns.Num();i++)
	{
		TArray<FMovementSequence> CaptureMoves = Board->CheckMoves(PlayerPawns[PlayerNumber].Pawns[i]->CellNumber, ActualPlayer,false);
		if(CaptureMoves.Num() > 0)
		{
			for (const FMovementSequence& Move : CaptureMoves)
			{
				PlayerPawns[PlayerNumber].Pawns[i]->CaptureMaxAllowed = Move.Cells.Num() / 2;
				
				if (Move.Cells.Num() > 1 && Move.Cells.Num() / 2 > CaptureMandatoryNumber)
				{
					CaptureMandatoryNumber = Move.Cells.Num()/2;
					// Capture maximale trouvée
					UE_LOG(LogTemp,Warning,TEXT("Find Capture for Pawn %d"),i);
				}
			}
		} else
		{
			PlayerPawns[PlayerNumber].Pawns[i]->CaptureMaxAllowed = 0;
		}
		
	}
	UE_LOG(LogTemp,Warning,TEXT("Capture number is : %d"), CaptureMandatoryNumber);
	for (int i = 0 ; i<PlayerPawns[PlayerNumber].Pawns.Num();i++)
	{
		if(PlayerPawns[PlayerNumber].Pawns[i]->CaptureMaxAllowed>0 && PlayerPawns[PlayerNumber].Pawns[i]->CaptureMaxAllowed == CaptureMandatoryNumber)
		{
			PlayerPawns[PlayerNumber].Pawns[i]->ChangeMaterial(true);
		}
	}
	
}

void ADameGameMode::SpawnPion(int32 CellNumber, int32 PlayerId)
{
	ADamePion* tempPion;
	FActorSpawnParameters parameters;
	FRotator myRot(0, 0, 0);

	tempPion = GetWorld()->SpawnActor<ADamePion>(Pion, Board->Cells[CellNumber]->GetActorLocation(), myRot, parameters);
	tempPion->CellNumber = CellNumber;
	tempPion->PlayerId = PlayerId;
	tempPion->Board = Board;
	tempPion->ChangeMaterial(false);
	PlayerPawns[(PlayerId == 1)?0:1].Pawns.Add(tempPion);
	Board->Cells[CellNumber]->Board = Board;
	Board->Cells[CellNumber]->PawnInCell = tempPion;
	
}
