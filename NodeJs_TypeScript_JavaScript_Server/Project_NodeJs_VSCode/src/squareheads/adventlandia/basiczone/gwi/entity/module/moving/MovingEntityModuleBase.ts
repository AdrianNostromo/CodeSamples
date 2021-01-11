import { ModuleLocal } from "../ModuleLocal";
import { MovingEntityTemplate } from "../../template/MovingEntityTemplate";
import { WorldEntityLocal } from "../../WorldEntityLocal";
import { MathUtil } from "base/util/MathUtil";
import { Vector3 } from "base/util/Vector3";
import { Quaternion3D } from "base/util/Quaternion3D";

export class MovingEntityModuleBase extends ModuleLocal {

  public readonly template: MovingEntityTemplate;

  private isMoveInProgress: boolean = false;
  private destMoveX: number;
  private destMoveY: number;

  private isTurnInProgress: boolean = false;
  private destTurnDZ: number;

  private isMovementStopInProgress: boolean = false;
  
  public constructor(entity: WorldEntityLocal, template: MovingEntityTemplate) {
    super(template.moduleId, entity, true);

    this.template = template;
  }
  
  public getIsMoveInProgress(): boolean {
    return this.isMoveInProgress;
  }
  
  public getDestMoveX(): number {
    return this.destMoveX;
  }
  
  public getDestMoveY(): number {
    return this.destMoveY;
  }

  public getIsTurnInProgress(): boolean {
    return this.isTurnInProgress;
  }
  
  public getDdestTurnDZ(): number {
    return this.destTurnDZ;
  }

  public getIsMovementStopInProgress(): boolean {
    return this.isMovementStopInProgress;
  }
  
  public move(destMoveX: number, destMoveY: number): void {
    if(this.isMoveInProgress && this.destMoveX == destMoveX && this.destMoveY == destMoveY) {
      // Ignore duplicate.
      
      return ;
    }

    this.destMoveX = destMoveX;
    this.destMoveY = destMoveY;
    
    this.isMoveInProgress = true;
  }
  
  public stopMovement(): void {
    if(this.isMovementStopInProgress) {
      // Ignore duplicate.
      
      return ;
    }

    // Stop the actual action.
    this.isMovementStopInProgress = true;
  }
  
  public turn(destTurnDZ: number): void {
    if(this.isTurnInProgress && this.destTurnDZ == destTurnDZ) {
      // Ignore duplicate.
      
      return ;
    }

    this.destTurnDZ = destTurnDZ;
    
    this.isTurnInProgress = true;
  }

  public gameLoop(deltaS: number, deltaMS: number): void {
    super.gameLoop(deltaS, deltaMS);

    if(this.isMovementStopInProgress) {
      this.isMovementStopInProgress = false;

      if(this.isMoveInProgress) {
        this.destMoveX = this.getEntity().getX();
        this.destMoveY = this.getEntity().getY();
        
        this.isMoveInProgress = false;
      }
    }

    if(this.isMoveInProgress) {
      let currentX: number = this.getEntity().getX();
      let currentY: number = this.getEntity().getY();
  
      let tickMoveAmount: number = this.template.moveSpeedS * deltaS;
  
      let delta: number = MathUtil.dist(
          currentX, currentY,
          this.destMoveX, this.destMoveY
      );
  
      let newX: number;
      let newY: number;
  
      let angRZ: number;
      if(currentX == this.destMoveX && currentY == this.destMoveY) {
          angRZ = MathUtil.toRadians(this.getEntity().getRot().getAngleAroundAxis(Vector3.Z));
      }else {
          angRZ = Math.atan2(this.destMoveY - currentY, this.destMoveX - currentX);
      }
  
      if(tickMoveAmount >= delta) {
          newX = this.destMoveX;
          newY = this.destMoveY;
      
          this.isMoveInProgress = false;
      }else {
          newX = currentX + Math.cos(angRZ) * tickMoveAmount;
          newY = currentY + Math.sin(angRZ) * tickMoveAmount;
      }
  
      // This will not cause the snapshot flag to be set for movement 
      // because the movement flag update handles that.
      this.getEntity().setPosXYZ(
        newX, 
        newY, 
        this.getEntity().getZ(),
        true
      );
      if(!this.isTurnInProgress) {
        // This will not cause the snapshot flag to be set for movement 
        // because the movement flag update handles that.
        this.getEntity().setRot(
          Quaternion3D.fromAxisAngle(Vector3.Z, MathUtil.toDegrees(angRZ)),
          true
        );
      }
    }

    if(this.isTurnInProgress) {
      this.isTurnInProgress = false;

      this.getEntity().setRotAxisAngle(
        Vector3.Z, this.destTurnDZ,
        true
      );
    }
  }
  
}
