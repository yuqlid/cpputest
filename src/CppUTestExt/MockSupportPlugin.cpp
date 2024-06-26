/*
 * Copyright (c) 2007, Michael Feathers, James Grenning and Bas Vodde
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the <organization> nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE EARLIER MENTIONED AUTHORS ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <copyright holder> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "CppUTestExt/MockSupportPlugin.h"

class MockSupportPluginReporter : public MockFailureReporter
{
    UtestShell& test_;
    TestResult& result_;
public:
    MockSupportPluginReporter(UtestShell& test, TestResult& result)
        : test_(test), result_(result)
    {
    }

    virtual void failTest(const MockFailure& failure) CPPUTEST_OVERRIDE
    {
        result_.addFailure(failure);
    }

    virtual UtestShell* getTestToFail() CPPUTEST_OVERRIDE
    {
        return &test_;
    }
};

MockSupportPlugin::MockSupportPlugin(const SimpleString& name)
    : TestPlugin(name)
{
}

MockSupportPlugin::~MockSupportPlugin()
{
    clear();
}

void MockSupportPlugin::clear()
{
    repository_.clear();
}

void MockSupportPlugin::preTestAction(UtestShell&, TestResult&)
{
    mock().installComparatorsAndCopiers(repository_);
}

void MockSupportPlugin::postTestAction(UtestShell& test, TestResult& result)
{
    MockSupportPluginReporter reporter(test, result);
    mock().setMockFailureStandardReporter(&reporter);
    if (!test.hasFailed())
        mock().checkExpectations();
    mock().clear();
    mock().setMockFailureStandardReporter(NULLPTR);
    mock().removeAllComparatorsAndCopiers();
}

void MockSupportPlugin::installComparator(const SimpleString& name, MockNamedValueComparator& comparator)
{
    repository_.installComparator(name, comparator);
}

void MockSupportPlugin::installCopier(const SimpleString& name, MockNamedValueCopier& copier)
{
    repository_.installCopier(name, copier);
}
