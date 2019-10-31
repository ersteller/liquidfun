
/**@constructor*/
function b2MutualForceController() {
    this.body = null;
    this.buffer = null;
    this.ptr = null;
    this.shape = null;
  }
  
var b2MutualForceController_Step =
    Module.cwrap('b2MutualForceController_Step', 'null', ['number']);
var b2MutualForceController_AddGroup =
    Module.cwrap('b2MutualForceController_AddGroup', 'null', ['number', 'number']);

b2MutualForceController.prototype.Step = function() {
    b2MutualForceController_Step(this.ptr);
};

b2MutualForceController.prototype.AddGroup = function(p) {
    b2MutualForceController_AddGroup(this.ptr, p);
};
  