import { NetCodeRanges } from "base/common/net/netcodes/NetCodeRanges";

export class NetCodes_Component_To_Component {

  // componentStatusID:int
  public static readonly REQUEST_COMPONENT_STATUS_CHANGED: number = NetCodeRanges.ComponentToComponent + 1;

  // void
  public static readonly RESPONSE_COMPONENT_STATUS_CHANGED_OK: number = NetCodeRanges.ComponentToComponent + 2;

}
