#ifndef STORM_THREAD_MAC_S_THREAD_RUNNER_H
#define STORM_THREAD_MAC_S_THREAD_RUNNER_H

#import <Foundation/Foundation.h>

struct SThreadParmBlock;

@interface SThreadRunner : NSObject

@property SThreadParmBlock* m_params;
@property(assign) NSAutoreleasePool* m_autoreleasePool;

- (instancetype)init:(SThreadParmBlock*)params;

- (void)run;

@end

#endif
