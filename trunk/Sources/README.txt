
Handling of different implementations:

- Non-static, Mutliple-platform, single implementation

  PObject and ObjectFactory
  
- Non-static, Single-platform, multiple implementations

  IAllocable, static New create method, only desired implementation file compiled
  
- Static, Mutliple-platform, single implementation

  Declaration in Core, implementation move to platform