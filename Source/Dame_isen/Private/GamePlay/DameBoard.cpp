// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlay/DameBoard.h"

#include "IPropertyTable.h"
#include "GamePlay/DameCase.h"
#include "GamePlay/DamePion.h"

// Sets default values
ADameBoard::ADameBoard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ADameBoard::IntializeBoard()
{

}


bool ADameBoard::IsOtherPlayerInCell(int32 cellNumber)
{
	return DameGM->GetPlayerIdByPawnId(DameGM->GetPawnIdByCellNumber(cellNumber)) != DameGM->GetActualPlayer();
}

bool ADameBoard::IsInNextLine(int32 cellNumber, int32 lineNUmber)
{
	return cellNumber/8 == lineNUmber;
}



TArray<FMovementSequence> ADameBoard::CheckMoves(int32 cellStart,int32 playerId, bool alreadyEat = false)
{
	TArray<FMovementSequence> validMoves;// On garde en mémoire les séquences pour déterminer les pions à prendre et proposer les endroits où on peut aller
	// Vérifier si le pion est une dame
    ADamePion* SelectedPion = DameGM->GetPawnByCellNumber(cellStart);
    if (SelectedPion && SelectedPion->IsQueen)
    {
    	FMovementSequence sequence;
    	validMoves = CheckDameMove(cellStart,playerId,alreadyEat,sequence);
    }
    else
    {
	    validMoves = CheckPawnMove(cellStart,playerId,alreadyEat);
    }
	
	return validMoves;
}

TArray<FMovementSequence> ADameBoard::CheckPawnMove(int32 cellStart, int32 playerId, bool alreadyEat )
{
	TArray<FMovementSequence> validMoves;// On garde en mémoire les séquences pour déterminer les pions à prendre et proposer les endroits où on peut aller
	// Vérifier si le pion est une dame
    // attention au numero du joueur ! le sens avant n'est pas le même !
	TArray<int32> movesToTest = {9*playerId,7*playerId};
	//On prépare le test pour savoir quel movement rapporte le plus.
	int maxCaptures = 0;
	for (int32 i = 0; i < movesToTest.Num(); i++)
	{
		int32 cellToTest = cellStart + movesToTest[i];

		//TODO : revoir aussi cette fonction pour les dames !
		//TODO : vérifier si je suis toujours sur le plateau (Penser à revoir le 64 si on veut jouer sur un plateau de taille différente

		if(cellToTest >=0 && cellToTest < 64 && IsInNextLine(cellToTest, cellStart/8+playerId))
		{
			if (IsOtherPlayerInCell(cellToTest))
			{
				// Ajout d'une variable intermédiaire pour rendre l'ensemble plus lisible
				int32 jumpCell = cellToTest + movesToTest[i];
				if(jumpCell <0 || jumpCell > 63)
				{
					break;
				}
				if(DameGM->IsCellEmpty(jumpCell) && IsInNextLine(jumpCell, cellToTest/8+playerId))
				{
					alreadyEat = true;

					//On cree une sequence de base : cell de départ, cell à tester et suivante si besoin. 
					TArray<int32> currentSequence = {cellToTest, jumpCell};
					//On fait notre recurence. 
					TArray<FMovementSequence> validMovesNext = CheckMoves(jumpCell,playerId,true);
					//On teste si on a capturer quelque chose
					if(validMovesNext.Num() > 0)
					{
						for(int j = 0; j < validMovesNext.Num(); j++)
						{
							TArray<int32> extendedSequence = currentSequence;
							extendedSequence.Append(validMovesNext[j].Cells);

							if (extendedSequence.Num() > maxCaptures)
							{
								maxCaptures = extendedSequence.Num();
								validMoves.Empty();

								FMovementSequence wrapper;
								wrapper.Cells = extendedSequence;
								validMoves.Add(wrapper);
							}
							else if (extendedSequence.Num() == maxCaptures)
							{
								FMovementSequence wrapper;
								wrapper.Cells = extendedSequence;
								validMoves.Add(wrapper);
							}
						}
					} else // Sinon On ne rajoute que ce mouvement là
					{
						if (currentSequence.Num() > maxCaptures)
						{
							maxCaptures = currentSequence.Num();
							validMoves.Empty();

							FMovementSequence wrapper;
							wrapper.Cells = currentSequence;
							validMoves.Add(wrapper);
						}
						else if (currentSequence.Num() == maxCaptures)
						{
							FMovementSequence wrapper;
							wrapper.Cells = currentSequence;
							validMoves.Add(wrapper);
						}
					}
				}
			}else if(!alreadyEat && DameGM->IsCellEmpty(cellToTest))
			{
				if (maxCaptures == 0)
				{
					FMovementSequence simpleMove;
					simpleMove.Cells = { cellToTest};
					validMoves.Add(simpleMove);
				}
			}  
		}
	}
    
	
	return validMoves;
}

TArray<FMovementSequence> ADameBoard::CheckDameMove(int32 cellStart, int32 playerId, bool alreadyEat, FMovementSequence prevMoves)
{
	TArray<FMovementSequence> validMoves;// On garde en mémoire les séquences pour déterminer les pions à prendre et proposer les endroits où on peut aller
	
	// Déplacements pour une dame en diagonale dans les quatre directions
    TArray<FVector2D> Directions = { FVector2D(1, 1), FVector2D(1, -1), FVector2D(-1, 1), FVector2D(-1, -1) };
    
    for (int h = 0 ; h < Directions.Num(); h++)
    {
    	
    		FMovementSequence Sequence;
	        bool CaptureHappened = false;
	        
	        for (int Step = 1; Step < 8; ++Step)
	        {
	            int32 TargetRow = (cellStart / 8) + Step * Directions[h].Y;
	            int32 TargetCol = (cellStart % 8) + Step * Directions[h].X;

	            if (TargetRow < 0 || TargetRow >= 8 || TargetCol < 0 || TargetCol >= 8)
	                break;  // Hors du plateau

	            int32 TargetCell = TargetRow * 8 + TargetCol;
				if (prevMoves.Cells.Contains(TargetCell))
				{
					break;
				}
	            if (!alreadyEat && DameGM->IsCellEmpty(TargetCell))
	            {
	                if (!CaptureHappened)  // Ajouter des mouvements jusqu'à ce qu'une capture soit faite
	                {
	                    Sequence.Cells.Add(TargetCell);
	                    validMoves.Add(Sequence);
	                }
	            }
	            else if (IsOtherPlayerInCell(TargetCell) && !CaptureHappened)
	            {
	                // Capture disponible
	                int32 AfterCaptureRow = TargetRow + Directions[h].Y;
	                int32 AfterCaptureCol = TargetCol + Directions[h].X;

	                if (AfterCaptureRow >= 0 && AfterCaptureRow < 8 && AfterCaptureCol >= 0 && AfterCaptureCol < 8)
	                {
	                    int32 AfterCaptureCell = AfterCaptureRow * 8 + AfterCaptureCol;
	                	if (prevMoves.Cells.Contains(AfterCaptureCell))
	                	{
	                		break;
	                	}
	                    if (DameGM->IsCellEmpty(AfterCaptureCell))
	                    {
	                        Sequence.Cells.Add(TargetCell);  // Case capturée
	                        Sequence.Cells.Add(AfterCaptureCell);  // Case après capture

	                        TArray<FMovementSequence> NewValidMoves = CheckDameMove(AfterCaptureCell,playerId,true,Sequence);
	                        if (NewValidMoves.Num() > 0)
	                        {
                        		FMovementSequence NewSequence;
                        		for(int j = 0 ; j < NewValidMoves.Num() ; j++)
                        		{
                        			NewSequence = Sequence;
                        			NewSequence.Cells.Append(NewValidMoves[j].Cells);
                        			validMoves.Add(NewSequence);
                        		}
	                        } else
	                        {
                        		validMoves.Add(Sequence);
	                        }
	                        
	                        
	                        CaptureHappened = true;
	                    }
	                }
	            }
	            else
	            {
	                break;  // Un pion allié ou une capture a déjà été faite
	            }
	        }
    	
        
    }
    int32 maxMoves = 0;
    for (int i = 0 ; i < validMoves.Num() ; i++)
    {
    	if(validMoves[i].Cells.Num() > 0 && validMoves[i].Cells.Num() > maxMoves)
    	{
    		maxMoves = validMoves[i].Cells.Num();
    	}
    }
    for (int i = 0 ; i < validMoves.Num();)
    {
    	if(validMoves[i].Cells.Num() < maxMoves)
    	{
    		validMoves.RemoveAt(i);
    	} else
    	{
    		i++;
    	}
    }

	return validMoves;
}

void ADameBoard::CellHighlight(int32 cellNumber,bool IsHighLighting)
{
	DameGM->GetCellByCellId(cellNumber)->ChangeMaterial(IsHighLighting);
}

void ADameBoard::NewPawnSelect(ADamePion* newPawn)
{
	if(PawnSelected != newPawn)
	{
		if(PawnSelected != nullptr)
		{
			PawnSelected->Unselect();
		}
		PawnSelected = newPawn;
		CellValidMoves = CheckMoves(DameGM->GetCellIdByPawnId(PawnSelected->PawnId),DameGM->GetPlayerIdByPawnId(PawnSelected->PawnId));
		if(CellValidMoves.Num() >0 && CellValidMoves[0].Cells.Num()/2 >= DameGM->CaptureMandatoryNumber)
		{
			UE_LOG(LogTemp,Warning,TEXT("Pawn selected can capture %d need %d"),CellValidMoves[0].Cells.Num()/2,DameGM->CaptureMandatoryNumber);
		
			for(int i = 0; i < CellValidMoves.Num(); i++)
			{
				//UE_LOG(LogTemp,Warning,TEXT("CaseSelectionnable : %i"),CellValidMoves[i]);
				CellHighlight(CellValidMoves[i].Cells.Last(),true);
			}
		} else
		{
			UE_LOG(LogTemp,Warning,TEXT("Pawn selected is not valid"));
			PawnSelected->Unselect();
			PawnSelected = nullptr;
			CellValidMoves.Empty();
		}
		
	}
	UE_LOG(LogTemp,Warning,TEXT("ADameBoard::NewPawnSelect %d"), CellValidMoves.Num());
}

void ADameBoard::NewCellSelect(class ADameCase* newCell)
{
	
	if(CellValidMoves.Num() == 0)
	{
		//TOdo : error catching
		return;
	}
	for(int i = 0; i < CellValidMoves.Num(); i++)
	{
		CellHighlight(CellValidMoves[i].Cells.Last());
		if(DameGM->GetCellByCellId(CellValidMoves[i].Cells.Last()) == newCell)
		{
			TArray<FVector> CellsPos;
			TArray<ADamePion*> OpponentPawns;
			if (CellValidMoves[i].Cells.Num() == 1)
			{
				CellsPos.Add(DameGM->GetCellByCellId(CellValidMoves[i].Cells[0])->GetActorLocation());
			}else
			{
				for(int j = 0; j < CellValidMoves[i].Cells.Num(); j++)
				{
					if (j%2 == 1)
					{
						FVector NewCellPos =DameGM->GetCellByCellId(CellValidMoves[i].Cells[j])->GetActorLocation() ;
						if (PawnSelected->IsQueen)
						{
							NewCellPos.Z += 50;
						}
						CellsPos.Add(NewCellPos);
					} else if (j%2 == 0)
					{
						OpponentPawns.Add(DameGM->GetPawnByCellNumber(CellValidMoves[i].Cells[j]));
					}
				}
			}
			DameGM->SetNewLink(PawnSelected->PawnId,CellValidMoves[i].Cells.Last());
			PawnSelected->StartMoveSequence(CellsPos,OpponentPawns);
		}
	}
}






// Called when the game starts or when spawned
void ADameBoard::BeginPlay()
{
	Super::BeginPlay();
	DameGM = Cast< ADameGameMode>( GetWorld()->GetAuthGameMode());
	DameGM->Board = this;
}

// Called every frame
void ADameBoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

