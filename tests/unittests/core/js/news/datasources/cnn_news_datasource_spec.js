"use strict";

const cnnNewsDataSource = require("js/news/datasources/cnn_news_datasource");
const newsExceptions = require("js/exceptions/invalidnews");
const fs = require("fs");

const InvalidNewsException = newsExceptions.InvalidNewsException;

describe("Tests suite for ensuring correct functionality for cnn datasource.", () => {
    beforeEach(() => {
        this._cnnDataSource = new cnnNewsDataSource.CnnNewsDataSource(this._httpClient);
        this._sampleContent = fs.readFileSync("tests/unittests/core/js/news/samples/cnn_sample_article.html").toString();

        expect(this._sampleContent).not.toBe(undefined);
    });


    it("CNN article parsed correctly.", () => {
        let news = this._cnnDataSource.parseContent(this._sampleContent);

        assertNewsModel(news);
    });

    it("CNN article parsed same result multiple parses.", () => {
        for (let idx = 0; idx < 10; idx++) {
            let news = this._cnnDataSource.parseContent(this._sampleContent);

            assertNewsModel(news);
        }
    });

    it("CNN article parse empty.", () => {
        expect(this._cnnDataSource.parseContent(undefined)).toBe(undefined);
        expect(this._cnnDataSource.parseContent("")).toBe(undefined);
        expect(this._cnnDataSource.parseContent("    ")).toBe(undefined);
    });

    it("CNN article parse incomplete.", () => {
        try {
            this._cnnDataSource.parseContent("invalid cnn article ...");
            expect(true).toBeFalsy();
        } catch(err) {
            expect(err instanceof InvalidNewsException).toBeTruthy();
            expect(err.cause).toBe("Invalid CNN article.");
            expect(err.message).toBe(InvalidNewsException.kDefaultMessage);
            expect(err.stack).not.toBe(undefined);
        }
    });

    /**
     * This function encapsulates the logic for correctly asserting the news logic against expected values.
     */
    let assertNewsModel = (news) => {
        expect(news).not.toBe(undefined);
        expect(news.url).toBe("http://www.cnn.com/2016/08/25/middleeast/muqawama-mosul-resistance-fighters/index.html");
        expect(news.headline).toBe("M for Muqawama: The secret resistance fighting ISIS with graffiti");
        expect(news.contributedBy).not.toBe(undefined);
        expect(news.contributedBy.length).toBe(1);
        expect(news.contributedBy[0]).toBe("CNN's Bryony Jones contributed to this report.");

        expect(news.paragraphs).not.toBe(undefined);
        expect(news.paragraphs.length).toBe(45);
        expect(news.paragraphs[0].content.trim()).toBe("\r\n".trim());
        expect(news.paragraphs[0].subheadingLevel).toBeFalsy();
        expect(news.paragraphs[1].content.trim()).toBe("The graffiti artist works under cover of darkness, spraying the letter M on walls across Mosul.");
        expect(news.paragraphs[1].subheadingLevel).toBeFalsy();
        expect(news.paragraphs[2].content.trim()).toBe("M for Muqawama: resistance; M for Muaarada: opposition; M for Muwajaha: confrontation.");
        expect(news.paragraphs[2].subheadingLevel).toBeFalsy();
        expect(news.paragraphs[3].content.trim()).toBe("This small act of rebellion is enough to leave the painter at risk of torture, even death.");
        expect(news.paragraphs[3].subheadingLevel).toBeFalsy();
        expect(news.paragraphs[4].content.trim()).toBe("Because that M is more than clandestine street art, more than simple vandalism. It is the Mosul Battalions' message to ISIS: We are here, we are among you, and we are watching.");
        expect(news.paragraphs[4].subheadingLevel).toBeFalsy();
        expect(news.paragraphs[5].content.trim()).toBe("Mosul has been under ISIS control since June 2014.");
        expect(news.paragraphs[5].subheadingLevel).toBeFalsy();
        expect(news.paragraphs[6].content.trim()).toBe("Iraq's army is fighting back; its troops are closing in on the city -- and say they hope to retake it by the end of the year. The Mosul Battalions are their allies inside the city.");
        expect(news.paragraphs[6].subheadingLevel).toBeFalsy();
        expect(news.paragraphs[7].content.trim()).toBe("The network -- so secretive that many of its own members do not know each other's identities -- carries out hit and run strikes, targeted assassinations, and bombings on ISIS targets in and around the city.");
        expect(news.paragraphs[7].subheadingLevel).toBeFalsy();
        expect(news.paragraphs[8].content.trim()).toBe("Risk of torture, death");
        expect(news.paragraphs[8].subheadingLevel).toBe(3);
        expect(news.paragraphs[9].content.trim()).toBe("Abu Ali (not his real name), who we meet in Erbil, is deliberately vague about his role in the Mosul Battalions, describing himself as a liaison to the organization.");
        expect(news.paragraphs[9].subheadingLevel).toBeFalsy();
        expect(news.paragraphs[10].content.trim()).toBe("He contacts a member of the group on our behalf; the man, an engineer by trade, tells us he is in the orchards on the outskirts of Mosul, one of the few areas of the city where there is cell phone reception away from ISIS spies.");
        expect(news.paragraphs[10].subheadingLevel).toBeFalsy();
        expect(news.paragraphs[11].content.trim()).toBe("Talking on a phone used to be a crime punishable by chopping the offender's hand off, he says. Now it means death.");
        expect(news.paragraphs[11].subheadingLevel).toBeFalsy();
        expect(news.paragraphs[12].content.trim()).toBe('"The group with me, we are all from Mosul," he says. "Saddam militarized the population, all Iraqi people have weapons training. But we do not have sophisticated weapons, we only have rifles and pistols."');
        expect(news.paragraphs[12].subheadingLevel).toBeFalsy();
        expect(news.paragraphs[13].content.trim()).toBe('He says the Battalions have support across the city: "Everyone gives us intelligence from their area. We conduct surveillance, we know when a guy has moved to a new place, when they are coming and going, we wait for the right time and carry it out."');
        expect(news.paragraphs[13].subheadingLevel).toBeFalsy();
        expect(news.paragraphs[14].content.trim()).toBe("On its Facebook page, the group posts details of its latest attacks, but getting messages out of Mosul is a challenge; at times the updates are a week apart.");
        expect(news.paragraphs[14].subheadingLevel).toBeFalsy();
        expect(news.paragraphs[15].content.trim()).toBe('"The Mosul Battalions men opened fire on a pick-up truck with a mounted machine gun with five ISIS members in it," the latest message reads. "The truck was demobilized, killing and wounding all of them."');
        expect(news.paragraphs[15].subheadingLevel).toBeFalsy();
        expect(news.paragraphs[16].content.trim()).toBe("Army's leftover weapons");
        expect(news.paragraphs[16].subheadingLevel).toBe(3);
        expect(news.paragraphs[17].content.trim()).toBe("The Battalions formed shortly after ISIS stormed into the city.");
        expect(news.paragraphs[17].subheadingLevel).toBeFalsy();
        expect(news.paragraphs[18].content.trim()).toBe('"It started as two friends who trust each other, and they would plot to hit ISIS at a particular point," says Abu Ali.');
        expect(news.paragraphs[18].subheadingLevel).toBeFalsy();
        expect(news.paragraphs[19].content.trim()).toBe('"There were others who had the same plans, and would hit ISIS in another area. Slowly they were able to organize themselves under the name of the Mosul Battalions."');
        expect(news.paragraphs[19].subheadingLevel).toBeFalsy();
        expect(news.paragraphs[20].content.trim()).toBe("The group's arms and ammunition come from within Mosul itself -- much of it left behind by the Iraqi army as they fled.");
        expect(news.paragraphs[20].subheadingLevel).toBeFalsy();
        expect(news.paragraphs[21].content.trim()).toBe('"There were weapons all over the city," says Abu Ali. "A lot of civilians were able to gather these weapons. ISIS did search and collect them from people\'s homes, but some were able to hide them."');
        expect(news.paragraphs[21].subheadingLevel).toBeFalsy();
        expect(news.paragraphs[22].content.trim()).toBe("Only three of the group's members know each other.");
        expect(news.paragraphs[22].subheadingLevel).toBeFalsy();
        expect(news.paragraphs[23].content.trim()).toBe('"They work in two person formations and a third person is at a higher level to avoid compromising the group if one is captured," he says.');
        expect(news.paragraphs[23].subheadingLevel).toBeFalsy();
        expect(news.paragraphs[24].content.trim()).toBe('Rebellion comes at a hefty cost: "Many of them have been captured by ISIS and killed by drowning, blowing them up, and slaughtering them in other brutal ways."');
        expect(news.paragraphs[24].subheadingLevel).toBeFalsy();
        expect(news.paragraphs[25].content.trim()).toBe("Roadside bomb attacks");
        expect(news.paragraphs[25].subheadingLevel).toBe(3);
        expect(news.paragraphs[26].content.trim()).toBe("Videos provided to CNN and others posted online show the capture and assassination of ISIS members, and attacks on isolated checkpoints and other targets, often using ISIS's own weapons against them.");
        expect(news.paragraphs[26].subheadingLevel).toBeFalsy();
        expect(news.paragraphs[27].content.trim()).toBe('"The roadside bombs they used, they would steal from ISIS." Abu Ali says. "ISIS puts bombs in certain areas and those who have previous military experience go and steal these bombs and place them to target ISIS."');
        expect(news.paragraphs[27].subheadingLevel).toBeFalsy();
        expect(news.paragraphs[28].content.trim()).toBe("Abu Ali refuses to disclose specific numbers but says the Battalions have anywhere between 100 and 300 fighters -- former professionals, ex-military personnel, and young people.");
        expect(news.paragraphs[28].subheadingLevel).toBeFalsy();
        expect(news.paragraphs[29].content.trim()).toBe("They are also -- through intermediaries like Abu Ali -- providing intelligence and coordinates on ISIS positions for coalition airstrikes.");
        expect(news.paragraphs[29].subheadingLevel).toBeFalsy();
        expect(news.paragraphs[30].content.trim()).toBe('"We have other means to verify the information they provide," Abu Ali says. "They wanted to work with the coalition for a couple reasons. So that the coalition is precise and doesn\'t hit civilian populations but also to accelerate the elimination of ISIS."');
        expect(news.paragraphs[30].subheadingLevel).toBeFalsy();
        expect(news.paragraphs[31].content.trim()).toBe("Waiting for 'Zero Hour'");
        expect(news.paragraphs[31].subheadingLevel).toBe(3);
        expect(news.paragraphs[32].content.trim()).toBe('Another member inside Mosul says the group has "two sides that we are working on. First of all, militarily against ISIS. Secondly we are making preparations for when the assault happens, we will facilitate from inside."');
        expect(news.paragraphs[32].subheadingLevel).toBeFalsy();
        expect(news.paragraphs[33].content.trim()).toBe('Once the Iraqi Army breaches the city, at what the battalion calls "Zero Hour," its members will intensify their attacks on ISIS. Another formation waiting to mobilize at "Zero Hour" calls itself the Peace Battalions. Its job is to mobilize and protect the public.');
        expect(news.paragraphs[33].subheadingLevel).toBeFalsy();
        expect(news.paragraphs[34].content.trim()).toBe('"Known people are leading them, they have the trust of the population," Abu Ali explains. "Their role is to take control of the streets and prevent looting, also they will become a supporting force to back up the liberating forces."');
        expect(news.paragraphs[34].subheadingLevel).toBeFalsy();
        expect(news.paragraphs[35].content.trim()).toBe('He says they want to avoid the sort of chaos seen in Baghdad after the US-led invasion in 2003, and to help prevent widespread death and destruction.');
        expect(news.paragraphs[35].subheadingLevel).toBeFalsy();
        expect(news.paragraphs[36].content.trim()).toBe('"There is a broad organization that will go into effect when the operations start to happen," he says. "The style of their clothes and the signals will be known so that they will be differentiated from the enemy."');
        expect(news.paragraphs[36].subheadingLevel).toBeFalsy();
        expect(news.paragraphs[37].content.trim()).toBe("Battle for control");
        expect(news.paragraphs[37].subheadingLevel).toBe(3);
        expect(news.paragraphs[38].content.trim()).toBe("Iraqi Army commanders tell us that they are counting on the population of the city -- groups like the Mosul Battalions and ordinary citizens -- to stand up against ISIS.");
        expect(news.paragraphs[38].subheadingLevel).toBeFalsy();
        expect(news.paragraphs[39].content.trim()).toBe("They say they need the support of the people to really succeed, not just in the upcoming battle for control of the city, but to prevent another ISIS (or worse) from emerging in the aftermath.");
        expect(news.paragraphs[39].subheadingLevel).toBeFalsy();
        expect(news.paragraphs[40].content.trim()).toBe("One Mosul Battalions member we spoke to via Facebook compared life under ISIS to life under former Shiite Prime Minister Nuri al-Maliki's army.");
        expect(news.paragraphs[40].subheadingLevel).toBeFalsy();
        expect(news.paragraphs[41].content.trim()).toBe("The fall of Mosul happened under his watch as did the alienation of the Sunni population brought on by the Iraqi security forces harsh hand and indiscriminate detention of Sunnis.");
        expect(news.paragraphs[41].subheadingLevel).toBeFalsy();
        expect(news.paragraphs[42].content.trim()).toBe("Those in the city say they do not want to live under either sort of rule.");
        expect(news.paragraphs[42].subheadingLevel).toBeFalsy();
        expect(news.paragraphs[43].content.trim()).toBe("One of the fighters we spoke to said the suggestion that citizens still in Mosul are supportive of ISIS is wrong.");
        expect(news.paragraphs[43].subheadingLevel).toBeFalsy();
        expect(news.paragraphs[44].content.trim()).toBe('"We want people to know that we reject ISIS," he said.');
        expect(news.paragraphs[44].subheadingLevel).toBeFalsy();
    };
});