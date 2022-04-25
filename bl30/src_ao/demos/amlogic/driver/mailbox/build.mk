#
#  Copyright (C) 2014-2018 Amlogic, Inc. All rights reserved.
#
#  All information contained herein is Amlogic confidential.
#
#  This software is provided to you pursuant to Software License Agreement
#  (SLA) with Amlogic Inc ("Amlogic"). This software may be used
#  only in accordance with the terms of this agreement.
#
#  Redistribution and use in source and binary forms, with or without
#  modification is strictly prohibited without prior written permission from
#  Amlogic.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
#  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
#  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
#  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
#  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
#  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
#  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
#  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
#  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
#  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
#  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#

ifeq ("$(SOC)","t3")
MAILBOX=y
endif
ifeq ("$(SOC)","p1")
MAILBOX=y
endif
ifeq ("$(SOC)","t7")
MAILBOX=y
endif
ifeq ("$(SOC)","s4")
MAILBOX=y
endif
ifeq ("$(SOC)","a5")
MAILBOX=y
endif
ifeq ("$(SOC)","sc2")
MAILBOX=y
endif
ifeq ("$(SOC)","t5")
MAILBOX_PL=y
endif
ifeq ("$(SOC)","t5d")
MAILBOX_PL=y
endif

ifeq ("$(SOC)","t5w")
MAILBOX_PL=y
endif

mailbox-$(MAILBOX) = mailbox.o mailbox-irq.o mailbox-htbl.o rpc-user.o
mailbox-$(MAILBOX_PL) = mailbox-pl.o mailbox-htbl.o rpc-user.o
