#include "thread/mac/SThreadRunner.h"
#include "thread/S_Thread.hpp"

@implementation SThreadRunner

- (instancetype)init:(SThreadParmBlock*)params {
    if (self = [super init]) {
        self.m_params = params;
        self.m_autoreleasePool = nullptr;
    }

    return self;
}

- (void)run {
    self.m_autoreleasePool = [[NSAutoreleasePool alloc] init];

    S_Thread::s_SLaunchThread(self.m_params);

    [self.m_autoreleasePool release];
}

@end
